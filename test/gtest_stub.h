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