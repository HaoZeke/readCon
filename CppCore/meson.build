_incdirs += [include_directories('thirdparty')]

# Source files
_srcs = ['helpers/FileHelpers.cc', 'helpers/StringHelpers.cc', 'ReadCon.cc']

if get_option('with_apache_arrow')
    _srcs += ['ConArrow.cc']
endif

# Create library
readconlib = library(
    'readcon',
    _srcs,
    dependencies: _deps,
    cpp_args: _args,
    link_with: _linkto,
    include_directories: _incdirs,
    install: true,
)
_linkto += readconlib

# --------------------- Executable

if not meson.is_subproject()
    tiny_cli = executable(
        'tiny_cli',
        'tiny_cli.cpp',
        dependencies: _deps,
        include_directories: _incdirs,
        cpp_args: _args,
        link_with: _linkto,
        install: false,
    )
endif

# ------------------------ Tests

if get_option('with_tests') and not is_windows
    catch_cpp = files('thirdparty/catch2/catch_amalgamated.cpp')
    subdir('tests')
endif
