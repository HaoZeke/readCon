project('readCon', 'cpp',
  version : '0.0.1',
  default_options : ['warning_level=1',
                     'cpp_std=c++17',
                     'wrap_mode=default'])


# Add C++ compiler options
_args = [] # Extra arguments
_deps = [] # Dependencies
_linkto = [] # All the sub-libraries
_incdirs = [] # All the includes

add_languages('c', required: true)
cc = meson.get_compiler('c')
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

# Faster, but optional due to size
if get_option('with_rangev3')
  ranges_dep = dependency('range-v3',
                          version : ['>=0.11.0'])
  if not ranges_dep.found()
    ranges_dep = dependency('ranges', subproject: subproject('ranges-v3'))
  endif
  _deps += ranges_dep
  _args += ['-DWITH_RANGE_V3']
endif

# Some helpers
if get_option('with_fmt')
  fmt_dep = dependency('fmt', version: '9.1.0')
  if not fmt_dep.found()
    fmt_dep = dependency('fmt', subproject: subproject('fmt'))
  endif
  _deps += fmt_dep
  _args += ['-DWITH_FMT']
endif


if get_option('with_apache_arrow')
  # Easiest to use within the conda environment, needs arrow-cpp
  arrow_dep = dependency('arrow', components: ['Arrow'], required : true)
  _deps += arrow_dep
  _args += ['-DWITH_ARROW']
endif

# --------------------- Library

subdir('CppCore') # defines readconlib
_incdirs += [ 'CppCore' ]
_linkto += readconlib
readcon_dep = declare_dependency(include_directories: _incdirs,
                                 link_with: _linkto,
                                 dependencies: _deps)
# ------------------------ Tests

if get_option('with_tests') and not is_windows
  gmock_dep = dependency('gmock', required: false)
  gtest_dep = dependency('gtest', main: true,)
  if not gtest_dep.found()
    gtest_dep = dependency('gtest', main: true,
                           subproject: subproject('gtest'))
  endif
  _deps += [ gtest_dep, gmock_dep ]
  _args += ['-DWITH_TEST']
test_array = [#
  ['ConFrame', 'testConFrame', 'TestConFrame.cc', ''],
  ['ConFrameVec', 'testConFrameVec', 'TestConFrameVec.cc', ''],
  ['ConFrameHelpers', 'testConFrameHelpers', 'TestConFrameHelpers.cc', ''],
             ]
foreach test : test_array
  test(test.get(0),
       executable(test.get(1),
          sources : ['CppCore/gtests/'+test.get(2)],
          dependencies : _deps,
          link_with : _linkto,
          cpp_args : _args,
          include_directories: _incdirs,
                 ),
        workdir : meson.source_root() + test.get(3)
      )
endforeach
endif
