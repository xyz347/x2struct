cc_library(
    name = 'x2struct',
    srcs = [
        'bson_builder.cpp',
        'bson_reader.cpp',
        'bson_writer.cpp',
        'config_reader.cpp',
        'config_writer.cpp',
        'json_reader.cpp',
        'json_writer.cpp',
        'util.cpp',
        'xml_reader.cpp',
        'xml_writer.cpp',
        'xtypes.cpp',
    ],
    deps = [
        '//thirdparty/libconfig:config++',
        '//thirdparty/mongo-cxx-driver:bson-1.0',
        '#rt',
    ],
    extra_cppflags = [
        '--std=c++11'
    ]
)


cc_test(
    name = 'x2struct_test',
    srcs = [
        'test/x2struct_test.cpp',
    ],
    incs = [
        './',
        '../',
    ],
    deps = [
        ':x2struct',
    ],
    extra_cppflags = [
        '--std=c++11',
        '-D__FULLPATH__=`readlink -f $SOURCE`'
    ]
)
