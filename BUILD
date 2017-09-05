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
        'xtypes.cpp',
    ],
    deps = [ # TODO modify here to correct path
        '//thirdparty/json:json',
        '//thirdparty/mongo:mongoclient',
        '//thirdparty/tinyxml:tinyxml',
        '//thirdparty/config++:config++',
        '//thirdparty/boost:boost_date_time',
        '//thirdparty/boost:boost_thread', # mongoclient need
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
        '-D__FULLPATH__=`readlink -f $SOURCE`',
        '-DUSE_BLADE',
    ]
)
