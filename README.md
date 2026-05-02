# libgbm
libgbm is the Generic Buffer Management loader, which various vendors rely on
to provide a GBM backend.

GBM is a specification that asynchronously allocate buffers and
retrieves objects for some components like eglImage. Its main purpose is for
use with OpenGL in some form, like OpenGL ES and EGL.

This is an up-to-date extraction of Mesa's libgbm.

## Dependencies
### Required
- A C compiler (cc/gcc/clang/etc.)
- pkg-config (or pkgconf)
- Meson >= 1.4.0
- Ninja (or samurai)
- libdrm >= 2.4.109
### Optional
- Python >= 3.x (for tests)
- nm (Binutils/LLVM; for tests)

## Operating System Support
Most systems making use of GBM are supported, except Android. Mesa supplies a
special libgbm for Android that is not meant to conflict with the one that
comes with each Android system. It's meant specifically for the Mesa drivers,
while this one is more general purposed and less specialized. Please rely on
Google's own offering. If you are installing Mesa for Android, use the libgbm
that comes from Mesa.

## Background
libgbm was developed as a part of Mesa. As Mesa has a large set of technologies
and existing ways of doing things, like functions, macros, and common headers,
the technology of libgbm dependended upon a long stream of functions and
references within a large utility library (`libmesa_util`), which has a lot of
dependencies.

This stream-like nature of libgbm was fixable but wouldn't make sense being
upstreamed. There are multiple reasons, however, to go Mesa-less. NVIDIA
systems, for example, have no use for Mesa if the proprietary userland video
driver is being used. libgbm is one of the major components that is still
needed, so making it Mesa-less by extracting it from its home serves this
purpose.

[This](https://gitlab.freedesktop.org/mesa/mesa/-/tree/main/src/gbm) is where
Mesa's libgbm implementation currently lives. This extraction is fully
ABI and API compliant.
