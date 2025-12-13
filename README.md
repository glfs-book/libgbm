# libgbm
libgbm is the Generic Buffer Management library, a frontend to GBM which
various vendors rely on to provide a GBM backend. In the case of Mesa, this
will be `dri_gbm`, and in the case of the NVIDIA driver, it will be
`nvidia-drm_gbm`, a symlink pointing to `libnvidia-allocator`.

# The purpose of this variation
libgbm is a part of the Mesa project. However, the `meson.build` file
for the project requires many options, dependencies, and targets to build just
the GBM frontend.

Other vendors will expect libgbm to be present, and often don't require
anything from Mesa but libgbm, it makes sense for libgbm to be isolated.

This project was extracted from Mesa
(https://gitlab.freedesktop.org/mesa/mesa/-/tree/main/src/gbm) and adapted for
simplicity. It is ABI compatible with Mesa's libgbm.

# Support
Android is not supported. However, other operating systems making use of GBM
are supported.
