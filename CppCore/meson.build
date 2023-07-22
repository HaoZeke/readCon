# Source files
src_files = [
  'helpers/FileHelpers.cc',
  'helpers/StringHelpers.cc',
  'ReadCon.cc',
]

_incdirs += include_directories(['include'])

# Create library
readconlib = library('readcon',
                 src_files,
                 dependencies: _deps,
                 cpp_args: _args,
                 link_with: _linkto,
                 install: false)
_linkto += readconlib

# --------------------- Executable

tiny_cli = executable('tiny_cli', 'tiny_cli.cpp',
                       dependencies: _deps,
                       include_directories: _incdirs,
                       cpp_args: _args,
                       link_with: _linkto,
                       install: false)
