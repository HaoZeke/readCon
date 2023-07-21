# Source files
src_files = [
  'BaseTypes.cc',
  'Helpers.cc',
  'ReadCon.cc',
]

_incdirs += include_directories(['include'])

# Create library
readconlib = library('readcon',
                 src_files,
                 dependencies: _deps,
                 cpp_args: _args,
                 link_with: _linkto,
                 install: true)

# --------------------- Executable

tiny_cli = executable('tiny_cli', 'tiny_cli.cpp',
                       dependencies: _deps,
                       include_directories: _incdirs,
                       link_with: _linkto,
                       install: false)
