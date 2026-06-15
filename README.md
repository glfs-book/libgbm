# libgbm
libgbm is the Generic Buffer Management backend loader, which various packages
and GBM vendors use.

GBM asynchronously allocates buffers and retrieves those buffers for projects
that need to access the Direct Rendering Manager. These allocations are often
for getting a buffer for a display, in which the project can send or get things
from it. This OS-portable (across UNIX operating systems) library loads
backends/drivers that do most of the work, and from there may be OS-specific
and may talk directly with the OS kernel. The main focus for GBM is said to be
OpenGL ES and EGL, but can be used for Vulkan, which may be necessary to make
the project portable, mainly for Wayland compositors. GBM is not tied to OpenGL
ES or EGL, but the backends it loads may be.

This is an up-to-date extraction of Mesa's libgbm.

> [!NOTE]
> This project does not install Mesa-specific interfaces or extensions, like
> Xorg DRI or `eglmesaext.h`. Projects that require them probably don't need to
> do so. `eglmesaext.h` used to have a lot more extenstions, but most of them
> have been upstreamed into the EGL registry. The Xorg DRI interface is only
> used by Xorg-Server, including the Xwayland branch.

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
UNIX operating systems that are supported by libdrm can use this library, but
the backend libgbm loads must support the OS and DRM system. Android, using a
Linux-based kernel, is not supported. Mesa supplies a special version of libgbm
for Android, and Android provides libgbm as well. Mesa's libgbm is meant
specifically for the Mesa drivers, while this one is more general purposed and
less specialized. Please rely on Google's own offering unless you are
installing Mesa on Android, in which case you should install libgbm from Mesa.

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
