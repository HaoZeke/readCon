project(
    'readCon',
    'cpp',
    version: '0.1.5',
    default_options: ['warning_level=1', 'cpp_std=c++17', 'wrap_mode=default'],
)

add_languages('c', 'cpp')

# Add C++ compiler options
_args = []  # Extra arguments
_linkto = []  # All the sub-libraries
_incdirs = []  # All the includes

add_languages('c', required: true)
cc = meson.get_compiler('c')
cppc = meson.get_compiler('cpp')

# Platform detection
host_system = host_machine.system()
is_windows = host_system == 'windows'
is_mingw = is_windows and cc.get_id() == 'gcc'

# Global C++ arguments
_global_cpp_args = cppc.get_supported_arguments(
    '-Wno-return-type',
    '-Wno-switch',
    '-Wno-unused-variable',
    '-Wno-unused-const-variable',
)
add_project_arguments(_global_cpp_args, language: 'cpp')

if is_mingw
# For mingw-w64, don't use LTO
    add_project_arguments('-fno-use-linker-plugin', language: ['c', 'cpp'])
endif

# Import the sourceset module
ssmod = import('sourceset')
ss = ssmod.source_set()

# --------------------- Dependencies
if get_option('with_rangev3')
    ranges_dep = dependency('range-v3', version: ['>=0.11.0'])
    if not ranges_dep.found()
        ranges_dep = dependency('ranges', subproject: subproject('ranges-v3'))
    endif
    ss.add(when: ranges_dep)
endif

if get_option('with_fmt')
    fmt_dep = dependency('fmt', version: '9.1.0')
    if not fmt_dep.found()
        fmt_dep = dependency('fmt', subproject: subproject('fmt'))
    endif
    ss.add(when: fmt_dep)
endif

if get_option('with_apache_arrow')
    arrow_dep = dependency('arrow', components: ['Arrow'], required: true)
    ss.add(
        when: arrow_dep,
        if_true: files('CppCore/ConArrow.cc', 'CppCore/helpers/ArrowHelpers.cc'),
    )
endif

if get_option('with_xtensor')
    xtensor_dep = dependency('xtensor')
    ss.add(
        when: xtensor_dep,
        if_true: files('CppCore/helpers/XtensorHelpers.cc'),
    )
endif

# --------------------- Library

_incdirs += [include_directories('CppCore')]
subdir('CppCore')  # defines readconlib

readcon_dep = declare_dependency(
    include_directories: _incdirs,
    link_with: _linkto,
    dependencies: readconss.dependencies(),
)
