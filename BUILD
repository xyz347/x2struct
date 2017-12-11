cc_library(
    name = 'x2struct',
    srcs = [
        'xobj.cpp',
        'jsonobj.cpp',
        'bsonobj.cpp',
        'xmlobj.cpp',
        'xstr.cpp',
        'configobj.cpp',
        'xmlstr.cpp',
        'jsoncfgstr.cpp',
        'bsonstr.cpp',
        'bsonbuilder.cpp',
        'xtypes.cpp',
        'util.cpp',
    ],
    deps = [
        '//thirdparty/json:json',
        '//thirdparty/tinyxml:tinyxml',
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
        '//thirdparty/boost:boost_system',
    ],
    extra_cppflags = [
        '--std=c++11',
        '-D__FULLPATH__=`readlink -f $SOURCE`'
    ]
)
