/*
* Copyright (C) 2017 YY Inc. All rights reserved.
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



#pragma once

#include <vector>
#include <iostream>

typedef void(*test_case)();

class TC_CONTAINER {
public:
    static void add(test_case tc) {
        I()._tcs.push_back(tc);
    }
    static const std::vector<test_case>& tcs() {
        return I()._tcs;
    }
    static TC_CONTAINER& I() {
        static TC_CONTAINER _inst;
        return _inst;
    }
private:
    std::vector<test_case> _tcs;
};

class AUTO_ADD_TC {
public:
    AUTO_ADD_TC(test_case tc) {
        TC_CONTAINER::add(tc);
    }
};

#define TEST(a, b) static void a##_##b();  static AUTO_ADD_TC __aat__##a##_##b(a##_##b); static void a##_##b()

#define EXPECT_EQ(a,b) \
do {\
    if (!((a)==(b))) {\
        std::cout<<std::endl<<"+++++++++++++++++++++++++++"<<__FILE__<<':'<<__LINE__<<'['<<__FUNCTION__<<']'<<"EXPECT_EQ fail."<<std::endl;\
    }\
}while(false)

#define EXPECT_TRUE(a) \
do {\
    if (!(a)) {\
        std::cout<<std::endl<<"+++++++++++++++++++++++++++"<<__FILE__<<':'<<__LINE__<<'['<<__FUNCTION__<<']'<<"EXPECT_TRUE fail."<<std::endl;\
    }\
}while(false)
    
