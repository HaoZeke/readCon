project('readCon', 'cpp',
  version : '1.5.3',
  default_options : ['warning_level=1',
                     'cpp_std=c++20',
                     'wrap_mode=forcefallback'])


# Add C++ compiler options
_args = [] # Extra arguments
_deps = [] # Dependencies
_linkto = [] # All the sub-libraries
_incdirs = [] # All the includes

cppc = meson.get_compiler('cpp')
# Platform detection
host_system = host_machine.system()
is_windows = host_system == 'windows'
is_mingw = is_windows and cc.get_id() == 'gcc'

# Conditional arguments
if host_system == 'linux'
  _args += '-Wno-return-type'
  _args += '-Wno-switch'
  _args += '-Wno-unused-variable'
  _args += '-Wno-unused-const-variable'
endif

cpu_family = host_machine.cpu_family()

if is_mingw
  # For mingw-w64, don't use LTO
  add_project_arguments('-fno-use-linker-plugin', language: ['c', 'cpp'])
endif

# --------------------- Dependencies
if not is_windows
  # libm for Unix systems
  m_dep = cppc.find_library('m', required: false)
  _deps += m_dep
  # For building with clang
  _deps += [declare_dependency(link_args: '-lstdc++')]
endif

if cppc.get_id() == 'msvc'
  # Don't depend on VCRUNTIME140_1.dll
  # https://cibuildwheel.readthedocs.io/en/stable/faq/#windows-importerror-dll-load-failed-the-specific-module-could-not-be-found
  add_project_arguments('/d2FH4-', language : ['cpp', 'c'])
  if get_option('with_stdcall')
    add_project_arguments(cppc.get_supported_arguments('/Gz'), language : ['cpp', 'c'])
  endif
endif

# Some helpers
if get_option('with_fmt')
  fmt_dep = dependency('fmt', required : false)
  _deps += fmt_dep
  _args += ['-DWITH_FMT']
endif

# --------------------- Library

subdir('CppCore') # defines readconlib
_linkto += readconlib
