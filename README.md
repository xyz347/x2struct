=x2struct

        用于解析 json xml libconfig bson 到C++结构体里面，也可以反过来。
        used for parse json xml libconfig bson to C++ struct or marshal C++ struct to json xml libconfig bson


使用方法：（范例请参考test/x2struct_test.cpp)
定义结构体时，在结构体结尾添加XTOSTRUCT宏定义，即可使用x2struct::X里面的函数序列化反序列化结构体。

比如
```C++
#include <string>
#include <iostream>
#include <vector>
#include "x2struct.hpp"

using namespace std;

// struct define
struct range {
    int min;
    int max;
    XTOSTRUCT(M(min, max)); // add line at end. include struct member is ok
};

struct example {
    int a;
    string b;
    std::vector<range> rs;
    XTOSTRUCT(A(a, "_id"), O(b, rs));
};


/*
 make # for generate libx2struct.a
 g++ -o t test.cpp libx2struct.a thirdparty/json/lib/libjson.a 
 ./t
*/
int main(int argc, char *argv[]) {
    // unmarshal
    
    /* parse a json file to struct. "a.json" is a json file
    example st1;
    x2struct::X::loadjson("a.json", st1, true);
    if (st1.xhas("rs")) { // check if a.json has key "rs" by xhas
    }*/

    // parse a json string to struct.
    example st2;
    x2struct::X::loadjson("{\"a\":1, \"b\":\"x2struct\", \"rs\":[{\"min\":1, \"max\":2}, {\"min\":10, \"max\":20}]}", st2, false);

    /* marshal struct to json string. parameter 3 control linefeed. if true, json string has linefeed, or will marshal
    to one line. parameter 4 only valid when true, control how man space add in line head.
    std::string json=x2struct::X::tojson(st2, "", true, 4); 
    */
    cout<<st2.a<<endl;
    cout<<st2.b<<endl;
    for (size_t i=0; i<st2.rs.size(); ++i) {
        cout<<st2.rs[i].min<<':'<<st2.rs[i].max<<endl;
    }
    return 0;
}
```

XTOSTRUCT 里面主要填struct里面各个变量的名称，需要放在下面三个字母之一里面：
O: optional，表示可选的，在反序列化的时候，如果这个字段不存在
   也是可以的，比如上述例子中，example.b, example.rs都是O，也
   即json里面没有这两个字段也是可以的
M: 与O对应，M表示必须存在对应的字段，比如range.min, range.max，
   如果json里面没这个字段，则会抛异常
A: 表示别名，用于X类型的key和结构体变量名不一样的情况。
   别名可以有：
       通用别名，对所有X类型生效，比如A(name, "_id")
       特定别名，只针对特定类型，比如A(name, "bson:_id")
   优先级是：特定别名>通用别名>原始名称


可以通过XTOSTRUCT_CONDITION/XTOSTRUCT_CONDITION_EQ进行条件反序列化（但是这样序列化出来的会和原来不一样)
具体可以参考x2struct_test.cpp里面的struct condition。

如果想实现一些自定义类型，可以在xtypes.hpp里面添加，具体可以参考XDate，要点：
1、继承XType
2、实现to_string，序列化
3、实现from_string，反序列化


【重要说明】
1、序列化部分并没有参考任何的RFC文档，都是按实际的开发需求写的，不一定符合标准
2、解析部分使用了下列开源代码：jsoncpp/tinyxml/mongodb.mongo/libconfig++
3、bson的反序列化/序列化用了mongodb.mongo，这个库比较重，为了减少依赖，编译的时候
   去掉了SSL功能，如果项目里面也用了这个库，可以修改指向。
4、里面的BUILD文件是针对使用blade编译的情况，需要适当修改deps
