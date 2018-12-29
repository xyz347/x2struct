# x2struct

*Read this in other languages: [简体中文](README.zh-cn.md).*

------

- Decode json/xml/libconfig/bson to C++ struct or encode C++ struct to json/xml/libconfig/bson
- Convert C++ struct define to Golang struct define(Incomplete, only for testing)

------
- Header file only
- bson/libconfig depend on lib file
- json is enabled by default, modify config.h to enable other:
    - xml need to define macro XTOSTRUCT_XML
    - bson need to define macro XTOSTRUCT_BSON
    - libconfig need to define macro XTOSTRUCT_LIBCONFIG
    - to generate Golang code, need to define macro XTOSTRUCT_GOCODE

------
- support vector/set/map
- support structure nesting

***
### EXAMPLE
Take a look at test/example_xxx.cpp or code below

```C++
// test.cpp

#include <string>
#include <iostream>
#include <vector>
#include "x2struct.hpp" // include this header file

using namespace std;

// struct define
struct range {
    int min;
    int max;
    XTOSTRUCT(M(min, max)); // add macro in the end of struct define
};

struct example {
    int a;
    string b;
    std::vector<range> rs;  // structure nesting is ok
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
    
    // xml/bson/libconfig is similar
    return 0;
}
```

### USAGE
add macro XTOSTRUCT in the end of struct define. contains all struct members
``` C++
struct example {
    int a;
    std::string b;
    std::vector<float> c;
    std::vector<int> d;
    XTOSTRUCT(A(a, "id"), O(b), M(c, d)); // add this line
};
```

Members must in one of A,M or O. A can only put one member at a time, and M/O can put multiple members.

- A: Alias. Used when the member name and key are inconsistent. such as member named UID, and key is "_id"<br>
  Generic alias  A(a, "_id")  "_id" effective for json/xml/bson/libconfig <br>
  Special alias A(a, "bson:_id")，"_id" only effective for bson. json/xml/libconfig use "a" <br>
  A(a, "id,bson:_id") bson use "_id"，others use "id"

- M: Mandatory. An exception will throw if no key found for mandatory member when decode
- O: Optional. Corresponding to M

***
Conditional decode can be done with XTOSTRUCT_CONDITION/XTOSTRUCT_CONDITION_EQ.
For details, please refer to the struct condition in x2struct_test.cpp.
***

If you want to implement some custom types, you can add them in xtypes.h. For details, please refer to XDate. key points:
- Define a class, implement format for encode, parse for decode
- Using the template XType typedef a type

### Local Class
Many compilers do not support defining template functions in the local class, while macro XTOSTRUCT defines template functions, so the macro XTOSTRUCT_NT is required in the local class. In addition, applying local class to template functions requires c++0x. support.

XTOSTRUCT_NT is used in the same way as XTOSTRUCT, and you need to specify which types of functions to generate by parameters. Support 4 types: Json/Bson/Xml/Config, such as XTOSTRUCT_NT(Json, Xml)(O(a,b,c), M(d));


### IMPORTANT
- Encode/decode json is use [rapidjson](https://github.com/Tencent/rapidjson)
- Decode xml is use [rapidxml](http://rapidxml.sourceforge.net)
- Decode bson is use [libbson](https://github.com/mongodb/libbson/tree/1.0.0)
- Decode libconfig is use [libconfig](https://github.com/hyperrealm/libconfig)
- Encode of xml/bson is written by myself. Without reference to the RFC, there may be cases where the standard is not met.
- The library of bson/libconfig is precompiled. The environment is: Ubuntu12.04 g++4.9.2. Other environments may need to download the code and recompile if you need to use these two libraries.
- bson/libconfig is not supported under Windows. If you need support, you have to download the code and compile to generate library file.
