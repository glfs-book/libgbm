# libgbm
The Generic Buffer Management Library

# Using at Runtime
Applications that use libgbm or those that utilize a library or two that use
**libgbm** can have some environment variables passed that determine how libgbm
behaves. The following describes those variables and their functions.

## `GBM_BACKENDS_PATH`
This **comma**-delimited (`arg1:arg2`) variable
specifies the paths which the GBM loader should look for GBM backends/drivers.

At compilation time, a default path was given to the loader. If there wasn't
an option passed to the build system for the default backend path, it should
have been `$PREFIX/$LIBDIR/gbm`. `GBM_BACKENDS_PATH` will prefix to the
overall search path before the default search path. In other words, the search
path would look like `GBM_BACKENDS_PATH:$PREFIX/$LIBDIR/gbm`.

An example on how to use the variable:

`GBM_BACKENDS_PATH=/usr/local/lib/gbm:/usr/lib/gbm2 eglinfo -B`

## `GBM_BACKEND`
This variable specifies which backend to load, which is
searched for in `GBM_BACKENDS_PATH` and in the default search path.

Each backend/driver should have a suffix (...`_gbm.so`). For example, the
most common backends look like `dri_gbm.so` and `nvidia-drm_gbm.so`.
`GBM_BACKEND` specifies the `dri` or `nvidia-drm` bit. In addition
to the variable, the loader looks for the name that's given by `libdrm` of
the automatically detected applicable card, given the DRM render node. This is
often the name of the kernel video driver, like `i915` and `nvidia-drm`.

If `GBM_BACKEND` is set to `new`, then **libgbm** will try to load
`new_gbm.so` in the search path. It will try to load that first before trying
to load the name that is given by **libdrm**. If that fails, then some backend
names are hardcoded in the loader to resort to:

- dri
- dumb

An example on how to use the variable:

`GBM_BACKEND=new eglinfo -B`
