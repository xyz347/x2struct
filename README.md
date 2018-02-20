x2struct
===========
用于解析 json/xml/libconfig/bson 到C++结构体里面，也可以反过来。

used for parse json/xml/libconfig/bson to C++ struct or marshal C++ struct to json/xml/libconfig/bson

***
### 范例 usage
参考 test/x2struct_test.cpp 或者下面的代码

see test/x2struct_test.cpp or code below

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

### 使用方法
在结构体定义的最后面添加一个XTOSTRUCT宏，里面包含结构体的各个变量即可。比如

add XTOSTRUCT at the end of struct, and put struct members in it. 
``` C++
struct example {
    int a;
    std::string b;
    std::vector<float> c;
    std::vector<int> d;
    XTOSTRUCT(A(a, "id"), O(b), M(c, d));
};
```

变量需要放在A/M/O三个字母之一里面，A每次只能放一个变量，M/O可以放多个变量。

- A: 表示别名，名用于key名称和变量名不一样的情况。比如变量名叫a，但是json里面的key是"id"，别名有<br>
  通用别名 A(a, "_id")  _id对json/xml/bson/libconfig都生效 <br>
  特定别名 A(a, "bson:_id")，只对bson生效，其他类型还是用的a <br>
  可以同时有多个 A(a, "id,bson:_id") 则bson用_id，其他用id

- M: M表示必须存在对应的字段，如果M(a)，那么对应的文件（比如json）必须存在a这个key，否则抛异常，M是与O对应。
- O: optional，表示可选的，在反序列化的时候，如果这个字段不存在也是可以的。O是与M相对应的

### usage
add XTOSTRUCT at the end of struct, and put struct members in it. 
``` C++
struct example {
    int a;
    std::string b;
    std::vector<float> c;
    std::vector<int> d;
    XTOSTRUCT(A(a, "id"), O(b), M(c, d));
};
```

member need to put in one of A/M/O

- A: alias name，for case struct member name not same as key name. <br>
  A(a, "_id")  _id work for json/xml/bson/libconfig <br>
  A(a, "bson:_id")，_id only work for bson, others use a <br>
  A(a, "id,bson:_id") bson use _id, others use id

- M: must apply，M(a), if a not exists in the bson/json/xml/libconfig, parse will throw an exception
- O: optional，in contrast to the M

***
可以通过XTOSTRUCT_CONDITION/XTOSTRUCT_CONDITION_EQ进行条件反序列化（但是这样序列化出来的会和原来不一样)
具体可以参考x2struct_test.cpp里面的struct condition。
***
如果想实现一些自定义类型，可以在xtypes.h里面添加，具体可以参考XDate，要点：
- 定义一个类，实现format用于序列化，parse用于反序列化
- 利用模板XType typedef一个类型


### 重要说明
- json的序列化和反序列化使用的是[rapidjson](https://github.com/Tencent/rapidjson)
- xml的解析使用的是[rapidxml](http://rapidxml.sourceforge.net)
- bson的解析用的是[libbson](https://github.com/mongodb/libbson/tree/1.0.0)
- 除了json以外，其余的序列化都是自己写的，没参考RFC，可能有不符合标准的情况
- 里面的BUILD文件是针对使用blade编译的情况，需要适当修改deps
