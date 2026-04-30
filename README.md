# libgbm
libgbm is the Generic Buffer Management library, a frontend to GBM which
various vendors rely on to provide a GBM backend. In the case of Mesa, this
will be `dri_gbm`, and in the case of the NVIDIA driver, it will be
`nvidia-drm_gbm`, a symlink pointing to `libnvidia-allocator`.

# The purpose of this variation
libgbm is a part of the Mesa project. However, the `meson.build` file
for the project requires many options, dependencies, and targets to build
the GBM frontend.

Since other vendors will expect libgbm to be present, and often don't require
anything from Mesa but libgbm, it makes sense for libgbm to be isolated.

This project was extracted from
[Mesa](https://gitlab.freedesktop.org/mesa/mesa/-/tree/main/src/gbm) and
adapted for simplicity. It is ABI and API compatible with Mesa's libgbm.

# What else is needed
A package providing drivers for OpenGL, X.org, GBM, Vulkan, OpenCL, and other
APIs should provide most of what you need on top of libgbm. However, there are
some files some packages expect that only come from Mesa:

- `$LIBDIR/pkgconfig/dri.pc`: A pkgconfig file saying the DRI interface is
  installed.
- `$INCLUDEDIR/GL/internal/dri_interface.h`: A header file declaring the DRI
  interface, doesn't need a library.

Sometimes, packages will also want the following:
- `$INCLUDEDIR/EGL/eglmesaext.h`: Contains Mesa-specific EGL extensions that
  have not yet been upstreamed.

`$INCLUDEDIR/EGL/eglmesaext.h` is nowadays almost empty as most Mesa extensions
got upstreamed by KhronosGroup. Please contact projects that include this file
as it is most likely not needed anymore.

A stub can be created for `$LIBDIR/pkgconfig/dri.pc`, which is what
[NixOS does](https://github.com/NixOS/nixpkgs/blob/master/pkgs/by-name/dr/dri-pkgconfig-stub/package.nix),
as well as [Gaming Linux From
Scratch](https://glfs-book.github.io/glfs/shareddeps/dri.html).

For the interface itself, only Mesa supplies it. Neither libglvnd nor this
project provide it. The good news is that it can simply be copied from the
Mesa tarball or downloaded from the FreeDesktop repository.

For the Mesa tarball, you can do something like this while in the root of the
extracted tarball directory (`mesa-<major>.<minor>.<patch>/.`), as the
***root*** user:
```Bash
install -vDm644 include/GL/internal/dri_interface.h \
        -t      $INCLUDEDIR/GL/internal/
```

For simply downloading the header, you can use `wget` (or `curl`) instead, as
the ***root*** user:
```Bash
install -vdm755 /usr/include/GL/internal &&
wget https://gitlab.freedesktop.org/mesa/mesa/-/raw/<major>.<minor>/include/GL/internal/dri_interface.h \
  -O /usr/include/GL/internal/dri_interface.h
```

# Operating System Support
Most systems making use of GBM are supported, except Android. Mesa supplies a
special libgbm for Android that is not meant to conflict with the one that
comes with each Android system. It's meant specifically for the Mesa drivers,
while this one is more general purposed and less specialized. Please rely on
Google's own offering. If you are installing Mesa for Android, use the libgbm
that comes from Mesa.
