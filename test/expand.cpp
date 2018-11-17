// for test macro expand

#include "../x2struct.hpp"

struct Test {
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    int g;
    XTOSTRUCT(O(a,b,c), A(b, "_id"), O(c,d), M(e,f,g));
};
