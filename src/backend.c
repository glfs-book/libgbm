#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <dlfcn.h>
#include <xf86drm.h>
#include "backend.h"

#define ARRAY_SIZE(a) (sizeof(a)/sizeof((a)[0]))
#define VER_MIN(a, b) ((a) < (b) ? (a) : (b))

/* For systems like Hurd */
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

struct gbm_backend_desc {
   const char *name;
   const struct gbm_backend *backend;
   void *lib;
};

#define BACKEND_LIB_SUFFIX "_gbm"

static void
free_backend_desc(const struct gbm_backend_desc *backend_desc)
{
   assert(backend_desc->lib);

   dlclose(backend_desc->lib);
   free((void *)backend_desc->name);
   free((void *)backend_desc);
}

static struct gbm_backend_desc *
create_backend_desc(const char *name,
                    const struct gbm_backend *backend,
                    void *lib)
{
   struct gbm_backend_desc *new_desc = calloc(1, sizeof(*new_desc));

   if (!new_desc)
      return NULL;

   new_desc->name = strdup(name);

   if (!new_desc->name) {
      free(new_desc);
      return NULL;
   }

   new_desc->backend = backend;
   new_desc->lib = lib;

   return new_desc;
}

static struct gbm_device *
backend_create_device(const struct gbm_backend_desc *bd, int fd)
{
   const uint32_t abi_ver = VER_MIN(GBM_BACKEND_ABI_VERSION,
                                    bd->backend->v0.backend_version);
   struct gbm_device *dev = bd->backend->v0.create_device(fd, abi_ver);

   if (dev) {
      if (abi_ver != dev->v0.backend_version) {
         _gbm_device_destroy(dev);
         return NULL;
      }
      dev->v0.backend_desc = bd;
   }

   return dev;
}

char path[PATH_MAX];
const char *next, *end;
void *driver = NULL;
const char *dl_error = NULL;
static struct gbm_device *
load_backend_by_name(const char *name, int fd)
{
   void *lib;
   end = DEFAULT_BACKENDS_PATH + strlen(DEFAULT_BACKENDS_PATH);
   for (const char *p = DEFAULT_BACKENDS_PATH; p < end; p = next + 1) {
      int len;
      next = strchr(p, ':');
      if (next == NULL)
         next = end;
      len = next - p;
      snprintf(path, sizeof(path), "%.*s/%s%s.so", len,
               p, name, BACKEND_LIB_SUFFIX);
      lib = dlopen(path, RTLD_NOW | RTLD_LOCAL);
      if (lib == NULL) {
         printf("GBM Loader: failed to open %s\n", path);
      }
      /* not need continue to loop all paths once the driver is found */
      if (lib != NULL)
         break;
   }
   if (lib == NULL) {
         printf("GBM Loader: failed to open %s: (search path: %s, suffix %s)\n",
              name, DEFAULT_BACKENDS_PATH, BACKEND_LIB_SUFFIX);
         return NULL;
   }
   printf("GBM Loader: dlopen(%s)\n", path);

   if (!lib)
      return NULL;

   struct gbm_device *dev = NULL;
   struct gbm_backend_desc *backend_desc;
   const struct gbm_backend *gbm_backend;
   GBM_GET_BACKEND_PROC_PTR get_backend;

   get_backend = dlsym(lib, GBM_GET_BACKEND_PROC_NAME);

   if (!get_backend)
      goto fail;

   gbm_backend = get_backend(&gbm_core);
   backend_desc = create_backend_desc(name, gbm_backend, lib);

   if (!backend_desc)
      goto fail;

   dev = backend_create_device(backend_desc, fd);

   if (!dev)
      free_backend_desc(backend_desc);

   return dev;

fail:
   dlclose(lib);
   return NULL;
}

struct gbm_device *
_gbm_create_device(int fd)
{
   struct gbm_device *dev = NULL;

   const char *b = getenv("GBM_BACKEND");
   if (b) {
      dev = load_backend_by_name(b, fd);
      if (dev) return dev;
   }

   drmVersionPtr v = drmGetVersion(fd);
   if (v) {
      dev = load_backend_by_name(v->name, fd);
      drmFreeVersion(v);
      if (dev) return dev;
   }

   dev = load_backend_by_name("dri", fd);
   if (dev) return dev;

   dev = load_backend_by_name("dumb", fd);
   if (dev) return dev;

   printf("%s\n%s\n%s\n%s\n%s\n",
"GBM Loader: All backends have failed to load.",
"            The proper backend for your driver may not be in place, or loading",
"            the correct one failed. Ensure your driver has modesetting on.",
"            Loading the dri GBM backend was a last resort and it may not mean",
"            it's the sole reason for a backend not being available for use.");
   return NULL;
}

void
_gbm_device_destroy(struct gbm_device *gbm)
{
   const struct gbm_backend_desc *backend_desc = gbm->v0.backend_desc;
   gbm->v0.destroy(gbm);

   if (backend_desc && backend_desc->lib)
      free_backend_desc(backend_desc);
}
