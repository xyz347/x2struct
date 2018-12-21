/*
* Copyright (C) 2019 YY Inc. All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License"); 
* you may not use this file except in compliance with the License. 
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, 
* software distributed under the License is distributed on an "AS IS" BASIS, 
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
* See the License for the specific language governing permissions and 
* limitations under the License.
*/



#ifndef __X_GTEST_STUB_H
#define __X_GTEST_STUB_H

#include <vector>
#include <iostream>

typedef void(*test_case)();

struct text_ctx {
    const char *group;
    const char *name;
    test_case tc;
};

class TC_CONTAINER {
public:
    static void add(test_case tc, const char* g, const char*n) {
        text_ctx tx;
        tx.group = g;
        tx.name = n;
        tx.tc = tc;
        I()._tcs.push_back(tx);
    }
    static const std::vector<text_ctx>& tcs() {
        return I()._tcs;
    }
    static TC_CONTAINER& I() {
        static TC_CONTAINER _inst;
        return _inst;
    }
private:
    std::vector<text_ctx> _tcs;
};

class AUTO_ADD_TC {
public:
    AUTO_ADD_TC(test_case tc, const char* g, const char*n) {
        TC_CONTAINER::add(tc, g, n);
    }
};

class Status {
public:
    static int&c() {
        static int _c;
        return _c;
    }
};

#define TEST(a, b) static void a##_##b();  static AUTO_ADD_TC __aat__##a##_##b(a##_##b, #a, #b); static void a##_##b()

#define EXPECT_EQ(a,b) \
do {\
    if (!((a)==(b))) {\
        std::cout<<std::endl<<"++++++++++"<<__FILE__<<':'<<__LINE__<<'['<<__FUNCTION__<<']'<<"EXPECT_EQ fail."<<std::endl;\
        std::cout<<"expect:"<<b<<std::endl;\
        std::cout<<"actual:"<<a<<std::endl;\
        ++Status::c();\
    }\
}while(false)

#define EXPECT_TRUE(a) \
do {\
    if (!(a)) {\
        std::cout<<std::endl<<"+++++++++++++++++++++++++++"<<__FILE__<<':'<<__LINE__<<'['<<__FUNCTION__<<']'<<"EXPECT_TRUE fail."<<std::endl;\
        ++Status::c();\
    }\
}while(false)


#endif

