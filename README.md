x2struct
====
*Read this in other languages: [简体中文](README.zh-cn.md).*


* Used to convert between C++ objects and json/xml/json/libconfig
* json/xml is header file only, no need to compile librarys
* json is enabled by default, others need to modify [config.h](config.h)
* The following example uses json as an example. Others can refer to api of x2struct::X in [x2struct.hpp](x2struct.hpp)
------
* [Basic usage](#basic-usage)
* [Mandatory](#mandatory)
* [Alias](#alias)
* [Inherit](#inherit)
* [Bit field](#bit-field)
* [Load conditional](#load-conditional)
* [char array](#char-array)
* [check exist](#check-exist)
* [local class](#local-class)
* [customize type](#customize-type)
* [xml bson libconfig](#xml-bson-libconfig)

Basic usage
----
```C++
#include <iostream>
#include "x2struct/x2struct.hpp" // include this header file

using namespace std;

struct User {
    int64_t id;
    string  name;
    string  mail;
    User(int64_t i=0, const string& n="", const string& m=""):id(i),name(n),mail(m){}
    XTOSTRUCT(O(id, name, mail)); // add XTOSTRUCT at the end of struct/class
};

struct Group {
    string  name;
    int64_t master;
    vector<User> members;
    XTOSTRUCT(O(name, master, members)); //  add XTOSTRUCT at the end of struct/class
};

int main(int argc, char *argv[]) {
    Group g;
    g.name = "C++";
    g.master = 2019;
    g.members.resize(2);
    g.members[0] = User(1, "Jack", "jack@x2struct.com");
    g.members[1] = User(2, "Pony", "pony@x2struct.com");

    string json = x2struct::X::tojson(g);  // C++ object to json
    cout<<json<<endl;

    Group n;
    x2struct::X::loadjson(json, n, false); // json to C++ object
    cout<<n.name<<endl;
    return 0;
}
```
- include "x2struct.hpp"
- add XTOSTRUCT at the end of struct, put all the variables inside "O"
- use x2struct::X::tojson to convert C++ object to json
- use x2struct::X::loadjson to load json to C++ object

Mandatory
----
- for json to C++ object
- use M to include those mandatory variables
- if mandatory field miss in json, an exception will throw

```C++
#include <iostream>
#include "x2struct/x2struct.hpp"

using namespace std;

struct Test {
    int64_t id;
    string  name;
    XTOSTRUCT(M(id), O(name)); // "id" is mandatory and "name" is optional
};

int main(int argc, char *argv[]) {
    Test t;
    string json="{\"name\":\"Pony\"}";

    x2struct::X::loadjson(json, t, false); // will throw exception(id is mandatory and no "id" in json)
    return 0;
}
```

Alias
----
- used for scenes where the variable name and json key name are inconsistent
- use A to include alias variable. "A" take two parameters, 1st is variable, 2nd is tag-field
- tag format is "[type:]alias[,extension]", Tag-field can contain multiple tags, separated by spaces
- type if one of "json"/"xml"/"bson"/"config", no type field in tag  means this tag is applied to all type
    - "tid" means use tid as alias for all type
    - "tid json:xid" means json use "xid" as alias and other types use "tid"
- extension only support "m" now, means mandatory(A like O, optional)
- if no alias in json, loadjson will also try variable name
- tojson use alias

``` C++
#include <iostream>
#include "x2struct/x2struct.hpp"

using namespace std;

struct Test {
    int64_t uid;
    string  name;
    XTOSTRUCT(A(uid, "id"), O(name)); // "uid" use alias "id"
};

int main(int argc, char *argv[]) {
    Test t;
    string json="{\"id\":123, \"name\":\"Pony\"}";

    x2struct::X::loadjson(json, t, false); 
    cout<<t.uid<<endl;
    return 0;
}

```

Inherit
----
- use I to include parents classs

```C++
#include <iostream>
#include "x2struct/x2struct.hpp"

using namespace std;

struct P1 {
    string mail;
    XTOSTRUCT(O(mail));
};

struct P2 {
    int64_t version;
    XTOSTRUCT(O(version));
};

struct Test:public P1, public P2 {
    int64_t uid;
    string  name;
    XTOSTRUCT(I(P1, P2), O(uid, name)); 
};

int main(int argc, char *argv[]) {
    Test t;
    string json="{\"mail\":\"pony@x2struct.com\", \"version\":2019, \"id\":123, \"name\":\"Pony\"}";

    x2struct::X::loadjson(json, t, false);
    cout<<t.mail<<endl;
    cout<<t.version<<endl;
    return 0;
}

```

Bit field
----
- use B to include those bit field variables

``` C++
#include <iostream>
#include "x2struct/x2struct.hpp"

using namespace std;

struct Test {
    int16_t ver:8;
    int16_t len:8;
    string  name;
    XTOSTRUCT(B(ver, len), O(name)); 
};

int main(int argc, char *argv[]) {
    Test t;
    string json="{\"ver\":4, \"len\":20, \"name\":\"IPv4\"}";

    x2struct::X::loadjson(json, t, false);
    cout<<t.ver<<endl;
    cout<<t.len<<endl;
    return 0;
}
```

Load conditional
----
- load at most one object from an array of objects
- use C to include those conditional variables at the beginning of XTOSTRUCT
- conditional variables still need included in one of O,A or M

```C++
#include <iostream>
#include "x2struct/x2struct.hpp"

using namespace std;

struct Task {
    int id;
    string name;
    XTOSTRUCT(O(id, name));
};

struct Test {
    int  tid;
    Task task;
    XTOSTRUCT(C(task), O(tid, task)); // task included in C and O, C must at beginning of XTOSTRUCT
    /*
     XTOSTRUCT_CONDITION take two parameters, 1st is class name, 2nd is variable name
     XTOSTRUCT_CONDITION defined a function take one parameter named "obj", pointer to xdoc of variable
     "task" in json is array, load conditional only load the one that this functin return true
    */
    XTOSTRUCT_CONDITION(Test, task) {
        int id;
        // only load the one that Task.id is equal to Test.tid
        return obj.convert("id", id)&&id==this->tid;
    }
};



int main(int argc, char *argv[]) {
    string s = "{\"tid\":2019,\"task\":[{\"id\":2018,\"name\":\"hello\"},{\"id\":2019,\"name\":\"world\"}]}";
    Test t;
    x2struct::X::loadjson(s, t, false);
    cout<<t.task.name<<endl;
}
```

char array
----
modify [config.h](config.h) to enable XTOSTRUCT_SUPPORT_CHAR_ARRAY


check exist
----
- to check if field exist after load json to C++ object
- use xhas to check if field exist

```C++
#include <iostream>
#include "x2struct/x2struct.hpp"

using namespace std;

struct Test {
    int64_t id;
    string  name;
    XTOSTRUCT(O(id, name));
};

int main(int argc, char *argv[]) {
    Test t;
    string json="{\"name\":\"Pony\"}";

    x2struct::X::loadjson(json, t, false, true); // 4th parameter apply true
    cout<<t.xhas("id")<<endl;       // use xhas to check exist
    cout<<t.xhas("name")<<endl;
    return 0;
}
```

local class
----


customize type
----


xml bson libconfig
----


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
- support inheritance. [example](test/inheritance.cpp)

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
- I: Inheritance. If has base class and base class defined XTOSTRUCT, use I to include the base class. [example](test/inheritance.cpp)
- B: For bit field, optional
- C: Load conditional. load one object from array.

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

