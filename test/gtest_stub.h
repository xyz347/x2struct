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



#ifndef __GTEST_STUB_H
#define __GTEST_STUB_H

#include <iostream>

#define TEST(a, b) static void a##_##b()

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
    

#endif