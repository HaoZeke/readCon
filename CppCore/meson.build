# Add include directories
_incdirs += [include_directories('thirdparty')]

# Add unconditional source files
ss.add(files('ReadCon.cc', 'helpers/FileHelpers.cc', 'helpers/StringHelpers.cc'))

# Apply the source set configuration
config = configuration_data()
config.set('WITH_RANGE_V3', get_option('with_rangev3'))
config.set('WITH_FMT', get_option('with_fmt'))
config.set('WITH_APACHE_ARROW', get_option('with_apache_arrow'))
config.set('WITH_XTENSOR', get_option('with_xtensor'))

readconconf = configure_file(output: 'readcon_conf.h', configuration: config)

ss.add(readconconf)

readconss = ss.apply(config)


# Create library
readconlib = library(
    'readcon',
    readconss.sources(),
    dependencies: readconss.dependencies(),
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
        dependencies: readconss.dependencies(),
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
