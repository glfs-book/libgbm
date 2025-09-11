# Copyright © 2017 Intel Corporation
# SPDX-License-Identifier: MIT

project(
  'libgbm',
  'c',
  version : '1.0.0',
  license : 'MIT',
  meson_version : '>= 1.4.0',
  default_options : [
  'buildtype=debugoptimized',
  'b_ndebug=if-release'],
)
cc = meson.get_compiler('c')
pre_args = [
  '-D__STDC_CONSTANT_MACROS',
  '-D__STDC_FORMAT_MACROS',
  '-D__STDC_LIMIT_MACROS',
  '-DPACKAGE_VERSION="@0@"'.format(meson.project_version()),
  '-DPACKAGE_BUGREPORT="https://github.com/glfs-book/libgbm/issues"',
]
c_args = []

inc_gbm = include_directories('src')

files_gbm = files(
  'src/backend.c',
  'src/backend.h',
  'src/gbm.c',
  'src/gbm.h',
  'src/gbmint.h',
)

gbm_backends_path = get_option('gbm-backends-path')
if gbm_backends_path == ''
  gbm_backends_path = join_paths(get_option('prefix'), get_option('libdir'), 'gbm')
endif

args_gbm = [
  '-DDEFAULT_BACKENDS_PATH="@0@"'.format(gbm_backends_path),
]

libgbm_name = 'gbm'

gbm_priv_libs = []
null_dep = dependency('', required : false)
dep_dl = null_dep
if host_machine.system() != 'windows'
  if not cc.has_function('dlopen')
    dep_dl = cc.find_library('dl', required : true)
  endif
  if cc.has_function('dladdr', dependencies : dep_dl)
    pre_args += '-DHAVE_DLADDR'
  endif
endif 
if dep_dl.found()
  gbm_priv_libs += '-ldl'
endif
_drm_ver = '2.4.109'
dep_libdrm = dependency('libdrm', version : '>=' + _drm_ver)
ld_args_gc_sections = []
libgbm = shared_library(
  libgbm_name,
  files_gbm,
  include_directories : inc_gbm,
  c_args : [args_gbm],
  link_args : [ld_args_gc_sections],
  dependencies : [dep_libdrm],
  gnu_symbol_visibility : 'hidden',
  version : '1.0.0',
  install : true,
)

install_headers('src/gbm.h', 'src/gbm_backend_abi.h')

pkg = import('pkgconfig')
pkg.generate(
  name : 'gbm',
  filebase : 'gbm',
  description : 'GBM library',
  version : meson.project_version(),
  libraries : libgbm,
  libraries_private : gbm_priv_libs,
  variables : ['gbmbackendspath=' + gbm_backends_path],
)

dep_gbm = declare_dependency(
  link_with : libgbm,
  include_directories : inc_gbm,
)
