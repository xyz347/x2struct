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
* [thirdparty class](#thirdparty-class)
* [Format indent](#format-indent)
* [xml bson libconfig](#xml-bson-libconfig)
* [Generate Golang struct](#generate-golang-struct)
* [IMPORTANT](#important)

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

    vector<int> vi;
    x2struct::X::loadjson("[1,2,3]", vi, false); // load vector directory
    cout<<vi.size()<<','<<vi[1]<<endl;

    map<int, int> m;
    x2struct::X::loadjson("{\"1\":10, \"2\":20}", m, false); // load map directory
    cout<<m.size()<<','<<m[2]<<endl;

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
- type is one of "json"/"xml"/"bson"/"config", no type field in tag  means this tag is applied to all type
    - for example, "tid" means use tid as alias for all type
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
- XTOSTRUCT will add some template function, and local class could not define template function
- use XTOSTRUCT_NT(types) instead of XTOSTRUCT, types support Json/Xml/Bson/Config
- need C++11 support
- not support load conditional now

```C++
// nt.cpp
// g++ -o t nt.cpp -std=c++11
#include <iostream>
#include "x2struct/x2struct.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    struct Test {
        int64_t id;
        string  name;
        XTOSTRUCT_NT(Json)(O(id, name));
    };

    Test t;
    string json="{\"id\":123, \"name\":\"Pony\"}";

    x2struct::X::loadjson(json, t, false);
    cout<<t.name<<endl;
    return 0;
}
```

customize type
----
- customize type is string
- need implement:
	- std::string format() const; use to format data to string
	- void parse(const std::string&); use to load string to data
- typedef XType<xxx> to define customize type
- following is an IPv4 example

```C++
#include <iostream>
#include "x2struct/x2struct.hpp"

using namespace std;

// just example, no error handle
struct _XIPv4 {
    uint32_t ip;
    std::string format() const {
        char buf[64];
        int  len = sprintf(buf, "%u.%u.%u.%u", ip>>24, 0xff&(ip>>16), 0xff&(ip>>8), 0xff&ip);
        return string(buf, len);
    }
    void parse(const std::string& d) {
        uint32_t u[4];
        sscanf(d.c_str(), "%u.%u.%u.%u", &u[0], &u[1], &u[2], &u[3]);
        ip = (u[0]<<24)+(u[1]<<16)+(u[2]<<8)+u[3];
    }
};

typedef x2struct::XType<_XIPv4> XIPv4;

struct Test {
    XIPv4 ip;
    XIPv4 mask;
    XTOSTRUCT(O(ip, mask));
};

int main(int argc, char *argv[]) {
    Test t;
    string json="{\"ip\":\"192.168.1.2\", \"mask\":\"255.255.255.0\"}";

    x2struct::X::loadjson(json, t, false);
    cout<<t.ip.ip<<','<<t.mask.ip<<endl;
    cout<<x2struct::X::tojson(t)<<endl;
    return 0;
}
```

Format indent
----
- last two parameters of tojson control format indent


thirdparty class
----
- need c++11 support
- use XTOSTRUCT_OUT instead of XTOSTRUCT

```c++
#include <sys/time.h>
#include <iostream>
#include "x2struct/x2struct.hpp"

using namespace std;

/*
struct timeval {
    time_t      tv_sec;
    suseconds_t tv_usec;
};
*/

// timeval is thirdparty struct
XTOSTRUCT_OUT(timeval, O(tv_sec, tv_usec));

struct T {
    int  a;
    string b;
    timeval t;
    XTOSTRUCT(O(a, b, t));
};


int main(int argc, char *argv[]) {
    T t;
    T r;
    t.a = 123;
    t.b = "x2struct";
    t.t.tv_sec = 888;
    t.t.tv_usec = 999;
    string s = x2struct::X::tojson(t);
    cout<<s<<endl;
    x2struct::X::loadjson(s, r, false);
    cout<<r.a<<','<<r.b<<','<<r.t.tv_sec<<','<<r.t.tv_usec<<endl;
    return 0;
}
```

xml bson libconfig
----
- need to modify [config.h](config.h) to enable
- not support load vector map directory
- xml/libconfig not permit use digit as key, so if want to use map<int, xxx>, key need add 'x', e.g. x100
- mongoxclient(https://github.com/xyz347/mongoxclient) is a wrapper of mongo-cxx-driver use bson


Generate Golang struct
----
- use to generate Golang struct define by C++ struct
- need to define macro XTOSTRUCT_GOCODE
```C++
// go.cpp
// g++ -o t go.cpp -DXTOSTRUCT_GOCODE
#include <iostream>
#include "x2struct/x2struct.hpp"

using namespace std;

struct Test {
    int64_t id;
    vector<string>  names;
    XTOSTRUCT(O(id, names));
};

int main(int argc, char *argv[]) {
    Test t;

    cout<<x2struct::X::togocode(t, true, true, true)<<endl; // last 3 parameter to generate (json/bson/xml) tags
    return 0;
}

```

output is:
```go
type Test struct {
        Id      int64       `json:"id" bson:"id" xml:"id"`
        Names   []string    `json:"names" bson:"names" xml:"names"`
}
```

IMPORTANT
----
- Encode/decode json is use [rapidjson](https://github.com/Tencent/rapidjson)
- Decode xml is use [rapidxml](http://rapidxml.sourceforge.net)
- Decode bson is use [libbson](https://github.com/mongodb/libbson/tree/1.0.0)
- Decode libconfig is use [libconfig](https://github.com/hyperrealm/libconfig)
- Encode of xml/bson is written by myself. Without reference to the RFC, there may be cases where the standard is not met.
- The library of bson/libconfig is precompiled. The environment is: Ubuntu12.04 g++4.9.2. Other environments may need to download the code and recompile if you need to use these two libraries.

