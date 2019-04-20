x2struct
====
* 用于在C++结构体和json/xml/json/libconfig之间互相转换
* json/xml只需要头文件, 无需编译库文件
* json缺省支持，其他的需要修改[config.h](config.h)开启相应功能
* 以下例子全部以json为例，其他的类似，具体可以参考[x2struct.hpp](x2struct.hpp)里面x2struct::X的api定义

------
* [基本用法](#基本用法)
* [必需节点](#必需节点)
* [别名](#别名)
* [继承](#继承)
* [位域](#位域)
* [条件加载](#条件加载)
* [char数组](#char数组)
* [检查是否存在](#检查是否存在)
* [局部类](#局部类)
* [自定义类型](#自定义类型)
* [第三方类和结构体](#第三方类和结构体)
* [格式化缩进](#格式化缩进)
* [xml bson libconfig](#xml-bson-libconfig)
* [生成Golang结构体](#生成golang结构体)
* [重要说明](#重要说明)

基本用法
----
```C++
#include <iostream>
#include "x2struct/x2struct.hpp" // 包含这个头文件

using namespace std;

struct User {
    int64_t id;
    string  name;
    string  mail;
    User(int64_t i=0, const string& n="", const string& m=""):id(i),name(n),mail(m){}
    XTOSTRUCT(O(id, name, mail)); // 添加宏定义XTOSTRUCT在结构体定义结尾
};

struct Group {
    string  name;
    int64_t master;
    vector<User> members;
    XTOSTRUCT(O(name, master, members)); // 添加宏定义XTOSTRUCT在结构体定义结尾
};

int main(int argc, char *argv[]) {
    Group g;
    g.name = "C++";
    g.master = 2019;
    g.members.resize(2);
    g.members[0] = User(1, "Jack", "jack@x2struct.com");
    g.members[1] = User(2, "Pony", "pony@x2struct.com");

    string json = x2struct::X::tojson(g);  // 结构体转json
    cout<<json<<endl;

    Group n;
    x2struct::X::loadjson(json, n, false); // json转结构体
    cout<<n.name<<endl;

    vector<int> vi;
    x2struct::X::loadjson("[1,2,3]", vi, false); // 直接加载vector
    cout<<vi.size()<<','<<vi[1]<<endl;

    map<int, int> m;
    x2struct::X::loadjson("{\"1\":10, \"2\":20}", m, false); // 直接加载map
    cout<<m.size()<<','<<m[2]<<endl;

    return 0;
}
```
步骤有：
- 引用头文件 "x2struct.hpp"
- 添加宏定义XTOSTRUCT在结构体结尾, 里面用"O"包含所有变量
- 用x2struct::X::tojson将结构体转json
- 用x2struct::X::loadjson将json转结构体

必需节点
----
- 用于在json转结构体的时候，某些key是要求必须存在的
- 用"M"来包含必需节点的变量
- 如果json里面没有这些key，转结构体的时候会抛出异常

```C++
#include <iostream>
#include "x2struct/x2struct.hpp"

using namespace std;

struct Test {
    int64_t id;
    string  name;
    XTOSTRUCT(M(id), O(name)); // "id"是必需节点，用M包含，"name"是可选节点，用"O"包含
};

int main(int argc, char *argv[]) {
    Test t;
    string json="{\"name\":\"Pony\"}";

    x2struct::X::loadjson(json, t, false); // 这里将会抛异常，因为"id"是必需节点，但是json里面没有
    return 0;
}
```

别名
----
- 用于变量名和key名不一致的场景
- 用"A"包含需要设置别名的变量，"A"包含两个参数，参数1是变量名，参数2是别名信息
- 别名段的基本格式是"[type:]alias[,extension]", 别名信息可以包含多个别名段，别名段之间用空格隔开
- type的值可以是"json"/"xml"/"bson"/"config", 如果别名段没有type这个字段，则表示这个别名段对所有的type生效，例子：
    - "tid" 由于没有type，那么表示所有的type的别名都是"tid"
    - "tid json:xid" "tid"没指定type，xid指定了json，那么json用xid，其他的用tid
- 扩展字段目前只支持"m"，表示必需节点（缺省是可选）
- 如果在json里面没有别名，依然会尝试变量名
- 结构体转json的时候，用别名

``` C++
#include <iostream>
#include "x2struct/x2struct.hpp"

using namespace std;

struct Test {
    int64_t uid;
    string  name;
    XTOSTRUCT(A(uid, "id"), O(name)); // "uid"的别名是"id"
};

int main(int argc, char *argv[]) {
    Test t;
    string json="{\"id\":123, \"name\":\"Pony\"}";

    x2struct::X::loadjson(json, t, false); 
    cout<<t.uid<<endl;
    return 0;
}

```

继承
----
- 使用"I"来包含父类

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

位域
----
- 使用"B"来包含位域变量

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

条件加载
----
- 用于从json对象数组里面加载满足条件的对象，最多加载一个
- 用"C"包含需要条件加载的变量，放在XTOSTRUCT开头
- 除了用"C"包含，条件加载的变量还需要继续用O/A/M中的一个包含

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
    XTOSTRUCT(C(task), O(tid, task)); // 在XTOSTRUCT里面，task既包含在"C"里面，也包含在"O"里面
    /*
     XTOSTRUCT_CONDITION 这个宏包含两个参数，一个类名，一个变量名
     XTOSTRUCT_CONDITION 定义了一个函数，函数有一个变量，名为"obj"指向的是变量对应的xdoc
     "task"在json里面是一个数组，这里只加载那个XTOSTRUCT_CONDITION返回true的那个，这里当tid等于id时返回true
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

char数组
----
修改[config.h](config.h)，使能XTOSTRUCT_SUPPORT_CHAR_ARRAY这个宏即可


检查是否存在
----
- 用于json转结构体的时候，检查json里面的某个key是否存在
- 用xhas函数检查

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

    x2struct::X::loadjson(json, t, false, true); // 如果需要用这个功能，第四个参数需要是true
    cout<<t.xhas("id")<<endl;       // 用xhas检查是否存在
    cout<<t.xhas("name")<<endl;
    return 0;
}
```

局部类
----
- XTOSTRUCT会在结构体/类中添加模板函数，而局部类（定义在函数内的类）是不支持模板函数的
- 用XTOSTRUCT_NT(types) 替代XTOSTRUCT, types支持Json/Xml/Bson/Config
- 需要支持C++11
- 目前局部类还不支持条件加载

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

自定义类型
----
- 自定义类型的本质是string类型
- 需要实现这些函数:
	- std::string format() const; 用于将对象转为字符串
	- void parse(const std::string&); 用于将字符串转为对象
- 最后用typedef XType<xxx> 完成自定义类型的定义
- 以下代码是一个IPv4的例子

```C++
#include <iostream>
#include "x2struct/x2struct.hpp"

using namespace std;

// 只是一个范例，没做错误处理
struct _XIPv4 {
    uint32_t ip;
    // 实现format函数
    std::string format() const {
        char buf[64];
        int  len = sprintf(buf, "%u.%u.%u.%u", ip>>24, 0xff&(ip>>16), 0xff&(ip>>8), 0xff&ip);
        return string(buf, len);
    }
    // 实现parse函数
    void parse(const std::string& d) {
        uint32_t u[4];
        sscanf(d.c_str(), "%u.%u.%u.%u", &u[0], &u[1], &u[2], &u[3]);
        ip = (u[0]<<24)+(u[1]<<16)+(u[2]<<8)+u[3];
    }
};

// typedef 定义
typedef x2struct::XType<_XIPv4> XIPv4;

struct Test {
    XIPv4 ip;
    XIPv4 mask;
    XTOSTRUCT(O(ip, mask));
};

int main(int argc, char *argv[]) {
    Test t;
    // 在json里面自定义类型是string
    string json="{\"ip\":\"192.168.1.2\", \"mask\":\"255.255.255.0\"}";

    x2struct::X::loadjson(json, t, false);
    cout<<t.ip.ip<<','<<t.mask.ip<<endl;
    cout<<x2struct::X::tojson(t)<<endl;
    return 0;
}
```

第三方类和结构体
----
- 需要C++11支持
- 用XTOSTRUCT_OUT而非XTOSTRUCT

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

格式化缩进
----
- tojson的最后两个参数控制


xml bson libconfig
----
- 如果需要这些功能，需要修改[config.h](config.h)来开启
- 不支持直接转换vector/map之类的，需要放结构体里面
- 由于xml/libconfig不支持用纯数字作为key，所以如果需要用map<int, xxx>，那么key需要用x+数字的方式，比如x100
- mongoxclient(https://github.com/xyz347/mongoxclient)是对mongo-cxx-driver的一个封装


生成Golang结构体
----
- 用于将C++结构体转Golang结构体
- 需要定义宏XTOSTRUCT_GOCODE
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

    cout<<x2struct::X::togocode(t, true, true, true)<<endl; // 最后三个参数用于控制(json/bson/xml) tag的生成
    return 0;
}

```

输出是:
```go
type Test struct {
        Id      int64       `json:"id" bson:"id" xml:"id"`
        Names   []string    `json:"names" bson:"names" xml:"names"`
}
```

重要说明
----
- json的序列化反序列化用的是[rapidjson](https://github.com/Tencent/rapidjson)
- xml的反序列化用的是[rapidxml](http://rapidxml.sourceforge.net)
- bson的反序列化用的是[libbson](https://github.com/mongodb/libbson/tree/1.0.0)
- libconfig的反序列化用的是[libconfig](https://github.com/hyperrealm/libconfig)
- xml/bson的序列化是我自己写的，没有参考RFC，可能有和标准不一样的地方.
- bson/libconfig的库是预编译的. 环境是: Ubuntu12.04 g++4.9.2. 其他环境如果想使用bson/libconfig可能需要自行编译这两个库
- 有疑问可以加QQ群878041110
