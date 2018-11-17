x2struct
===========
- 用于将json/xml/libconfig/bson转为C++结构体，也可以反过来将结构体转成相应字符串
- 可用于将C++结构体生成golang的结构体(不完备，仅用于测试)

------
- json/xml 只有头文件，包含即可用
- bson/libconfig 有源文件，需要make生成libx2struct.a 
- 如果需要用bson功能，需要定义宏XTOSTRUCT_BSON
- 如果需要用libconfig功能，需要定义宏XTOSTRUCT_LIBCONFIG
- 如果需要生成golang代码，需要定义宏XTOSTRUCT_GOCODE

------
- 支持vector/set/map
- 支持结构体嵌套

***
### 范例
参考 test/example_xxx.cpp 或者下面的代码

```C++
// test.cpp

#include <string>
#include <iostream>
#include <vector>
#include "x2struct.hpp" // 增加这个头文件引用

using namespace std;

// struct define
struct range {
    int min;
    int max;
    XTOSTRUCT(M(min, max)); //在结构体后增加一行，将成员变量包含进去
};

struct example {
    int a;
    string b;
    std::vector<range> rs;  // 结构体支持嵌套
    XTOSTRUCT(A(a, "_id"), O(b, rs));
};


/*
 g++ -o t test.cpp
 ./t
*/
int main(int argc, char *argv[]) {
    example x;
    
    // json string to struct
    x2struct::X::loadjson("{\"a\":1, \"b\":\"x2struct\", \"rs\":[{\"min\":1, \"max\":2}, {\"min\":10, \"max\":20}]}", x, false);

    // struct to json string
    cout<<x2struct::X::tojson(x)<<endl;
    
    // xml/bson/libconfig可以调用类似接口进行转换
    return 0;
}
```

### 使用方法
在结构体定义的最后面添加一个XTOSTRUCT宏，里面包含结构体的各个变量即可。比如
``` C++
struct example {
    int a;
    std::string b;
    std::vector<float> c;
    std::vector<int> d;
    XTOSTRUCT(A(a, "id"), O(b), M(c, d)); // 只需要添加一个宏在结构体后面
};
```

变量需要放在A/M/O三个字母之一里面，A每次只能放一个变量，M/O可以放多个变量。

- A: 表示别名，名用于key名称和变量名不一样的情况。比如变量名叫a，但是json里面的key是"id"，别名有<br>
  通用别名 A(a, "_id")  _id对json/xml/bson/libconfig都生效 <br>
  特定别名 A(a, "bson:_id")，只对bson生效，其他类型还是用的a <br>
  可以同时有多个 A(a, "id,bson:_id") 则bson用_id，其他用id

- M: M表示必须存在对应的字段，如果M(a)，那么对应的文件（比如json）必须存在a这个key，否则抛异常，M是与O对应。
- O: optional，表示可选的，在反序列化的时候，如果这个字段不存在也是可以的。O是与M相对应的

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
- bson的解析使用的是[libbson](https://github.com/mongodb/libbson/tree/1.0.0)
- libconfig解析使用的是[libconfig](https://github.com/hyperrealm/libconfig)
- 除了json以外，其余的序列化都是自己写的，没参考RFC，可能有不符合标准的情况
- bson/libconfig的库是预先编译的，环境是：Ubuntu12.04 g++4.9.2，其他环境如果需要使用这两个库可能需要自己下载代码重新编译
- Windows下不支持bson/libconfig，如果需要支持，需要自行下载相应代码自行编译库文件，并修改makefile
- Windows下不支持XDate，如果需要支持，需要自行修改xtype.cpp
