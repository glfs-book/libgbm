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
- `$INCLUDEDIR/EGL/eglmesaext.h`: Describes the EGL Mesa EXT interface.
- `$INCLUDEDIR/EGL/eglext_angle.h`: The same as the previous one, but for
  ANGLE.

A stub can be created for `$LIBDIR/pkgconfig/dri.pc`, which is what
[NixOS does](https://github.com/NixOS/nixpkgs/blob/master/pkgs/by-name/dr/dri-pkgconfig-stub/package.nix),
as well as [Gaming Linux From
Scratch](https://glfs-book.github.io/glfs/shareddeps/dri.html).

For the header files, only Mesa supplies them. Neither libglvnd nor this
project provide them. The good news is that they can simply be copied from the
Mesa tarball or downloaded from the FreeDesktop repository.

For the Mesa tarball, you can do something like this while in the root of the
extracted tarball directory (`mesa-<major>.<minor>.<patch>/.`), as the
***root*** user:
```Bash
install -vDm644 include/GL/internal/dri_interface.h \
        -t      $INCLUDEDIR/GL/internal/           &&
install -vDm644 include/EGL/eglmesaext.h            \
        -t      $INCLUDEDIR/EGL/                   &&
install -vDm644 include/EGL/eglext_angle.h          \
        -t      $INCLUDEDIR/EGL/
```

For simply downloading the headers, you can use `wget` (or `curl`) instead, as
the ***root*** user:
```Bash
install -vdm755 /usr/include/GL/internal \
                /usr/include/EGL        &&
wget https://gitlab.freedesktop.org/mesa/mesa/-/raw/<major>.<minor>/include/GL/internal/dri_interface.h \
  -O /usr/include/GL/internal/dri_interface.h &&
wget https://gitlab.freedesktop.org/mesa/mesa/-/raw/<major>.<minor>/include/EGL/eglmesaext.h \
  -O /usr/include/EGL/eglmesaext.h &&
wget https://gitlab.freedesktop.org/mesa/mesa/-/raw/<major>.<minor>/include/EGL/eglext_angle.h \
  -O /usr/include/EGL/eglext_angle.h
```

# Operating System Support
Most systems making use of GBM are supported, except Android. Mesa supplies a
special libgbm for Android that is not meant to conflict with the one that
comes with each Android system. It's meant specifically for the Mesa drivers,
while this one is more general purposed and less specialized. Please rely on
Google's own offering. If you are installing Mesa for Android, use the libgbm
that comes from Mesa.
