# libgbm
libgbm is the Generic Buffer Management library, a frontend to GBM which
various vendors rely on to provide a GBM backend. In the case of Mesa, this
will be `dri_gbm`, and in the case of the NVIDIA driver, it will be
`nvidia-drm_gbm`, a symlink pointing to `libnvidia-allocator`.

It's a part of the Mesa project. However, the `meson.build` file
for the project is really complicated and requires several options just to
build the GBM frontend. Beyond that, it requires building a good lot of files
before getting to the ones that really matter.

For something so simple and that other vendors will expect to be present, it
makes sense to have it in its own simple stand-alone package.

This project was forked/ported from Mesa
(https://gitlab.freedesktop.org/mesa/mesa/-/tree/main/src/gbm) and adapted to
have less dependencies as well as be more simple and direct for what's needed.
