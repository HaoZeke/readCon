_args += ['-DWITH_TEST']
test_args = _args
test_array = [  #
    ['ConFrame', 'testConFrame', 'TestConFrame.cc', ''],
    ['ConFrameVec', 'testConFrameVec', 'TestConFrameVec.cc', ''],
    ['ConFrameHelpers', 'testConFrameHelpers', 'TestConFrameHelpers.cc', ''],
]
if get_option('with_xtensor')
    test_array += [
        ['XTensor Adapters', 'testXtensorAdapter', 'TestXTensorAdapter.cc', ''],
    ]
endif
foreach test : test_array
    test(
        test.get(0),
        executable(
            test.get(1),
            sources: [test.get(2), catch_cpp],
            dependencies: readconss.dependencies(),
            link_with: _linkto,
            cpp_args: _args,
            include_directories: _incdirs,
        ),
        workdir: meson.source_root() + test.get(3),
    )
endforeach
