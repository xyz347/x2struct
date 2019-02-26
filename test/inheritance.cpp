#include <iostream>
#include <map>

#include "x2struct/x2struct.hpp"

using namespace std;

struct Base {
    int a;
    int b;
    XTOSTRUCT(O(a,b));
};

struct Base2 {
    int d;
    XTOSTRUCT(O(d));
};

struct Child:public Base,public Base2 {
    int c;
    XTOSTRUCT(O(c), I(Base,Base2));
};


int main(int argc, char *argv[]) {
    Child c;
    c.a = 1;
    c.b = 2;
    c.c = 3;
    c.d = 4;
    string s = x2struct::X::tojson(c);
    cout<<s<<endl;
    Child d;
    x2struct::X::loadjson(s, d, false);
    cout<<d.a<<','<<d.b<<','<<d.c<<','<<d.d<<endl;
    return 0;
}
