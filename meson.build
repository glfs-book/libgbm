project(
  'libgbm',
  'c',
  version : files('VERSION'),
  license : 'MIT',
  meson_version : '>= 1.4.0',
  default_options : [
  'buildtype=debugoptimized',
  'b_ndebug=if-release'],
)
cc = meson.get_compiler('c')

mesa_track = '26.2'
_drm_ver = '2.4.109'

inc_gbm = include_directories('include')

files_gbm = files(
  'src/backend.c',
  'src/gbm.c',
)

gbm_backends_path = get_option('gbm-backends-path')
if gbm_backends_path == ''
  gbm_backends_path = join_paths(get_option('prefix'), get_option('libdir'), 'gbm')
endif

args_gbm = [
  '-DDEFAULT_BACKENDS_PATH="@0@"'.format(gbm_backends_path),
]

if (cc.has_header_symbol('sys/sysmacros.h', 'major') and
  cc.has_header_symbol('sys/sysmacros.h', 'minor') and
  cc.has_header_symbol('sys/sysmacros.h', 'makedev'))
  args_gbm += '-DMAJOR_IN_SYSMACROS'
endif
if (cc.has_header_symbol('sys/mkdev.h', 'major') and
  cc.has_header_symbol('sys/mkdev.h', 'minor') and
  cc.has_header_symbol('sys/mkdev.h', 'makedev'))
  args_gbm += '-DMAJOR_IN_MKDEV'
endif

libgbm_name = 'gbm'

gbm_priv_libs = []
null_dep = dependency('', required : false)
dep_dl = null_dep
if host_machine.system() != 'windows'
  if not cc.has_function('dlopen')
    dep_dl = cc.find_library('dl', required : true)
  endif
endif 
if dep_dl.found()
  gbm_priv_libs += '-ldl'
endif

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
  version : meson.project_version(),
  install : true,
)

if get_option('tests')
  abi_check = executable('gbm_abi_check', 'tests/gbm_abi_check.c')
  symbol_check = find_program(meson.current_source_dir() / 'tests/symbols-check.py')
  test('gbm-abi-check', abi_check, suite : ['gbm'])
  nm = find_program('nm', required : false)
  if nm.found()
    test('check-symbols', symbol_check,
      args :
        ['--nm', nm.full_path(),
         '--symbols-file', meson.current_source_dir() / 'tests/gbm-symbols.txt',
         '--lib', libgbm.full_path()],
      suite : ['gbm'])
  endif
endif

install_headers('include/gbm.h', 'include/gbm_backend_abi.h')
install_man('doc/libgbm.3')
install_data('doc/using.md',
  install_dir : join_paths(get_option('prefix'),
                get_option('datadir'))
                + '/doc/libgbm-'
                + meson.project_version()
)

pkg = import('pkgconfig')
pkg.generate(
  name : 'gbm',
  filebase : 'gbm',
  description : 'GBM library',
  version : mesa_track,
  libraries : libgbm,
  libraries_private : gbm_priv_libs,
  variables : ['gbmbackendspath=' + gbm_backends_path],
)

dep_gbm = declare_dependency(
  link_with : libgbm,
  include_directories : inc_gbm,
)
