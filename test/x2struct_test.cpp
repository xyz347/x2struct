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



#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>

#ifdef USE_BLADE
#include <gtest/gtest.h>
#include "test/blade_test_env.h"
#else
#include "test/gtest_stub.h"
#endif

#include "x2struct.hpp"
#include "xtypes.hpp"

#ifndef NO_BSON
#include "x2struct_bson.hpp"
#endif

using namespace std;
using namespace x2struct;

#ifdef USE_BLADE
  BLADE_TEST_COMMON_ENV;
#endif

struct sub {
    int s1;
    string s2;
    int s3;
    XTOSTRUCT(M(s1,s2),O(s3));
};

struct server {
    string host;
    uint16_t port;
    XTOSTRUCT(M(host,port));
};

struct condition {
    vector<server> redis;
    XTOSTRUCT(M(redis));
    XTOSTRUCT_CONDITION() {
        return obj.attribute("myip")==obj.attribute("cfgip");
    }
};

struct haha {
    int a;
    int b;
    int c;
    XTOSTRUCT(M(a,b));
};

struct JTest {
    XDate start;
    int a;
    string b;
    vector<sub> s;
    condition con;
    haha ha;
    vector<int> vi;
    vector<string> vs;
    JTest(){start.unix_time = 0;}
    XTOSTRUCT(A(a,"_id,me"),O(ha, start,con, b,s, vi, vs));
};

TEST(config, file)
{
    JTest j1;
    JTest j2;
    X::loadconfig("test1.cfg", j1, true);
    EXPECT_EQ(j1.start.unix_time, 1488528000);
    EXPECT_EQ(j1.a, 1);
    EXPECT_EQ(j1.b, "Hello");
    EXPECT_EQ(j1.s.size(), 2U);
    EXPECT_EQ(j1.s[0].s1, 2);
    EXPECT_EQ(j1.s[0].s2, "hahaha");
    EXPECT_EQ(j1.s[1].s1, 99);
    EXPECT_EQ(j1.s[1].s2, "nani");
    EXPECT_EQ(j1.con.redis.size(), 2U);
    EXPECT_EQ(j1.con.redis[0].host, "2.1.1.1");
    EXPECT_EQ(j1.con.redis[0].port, 200U);
    EXPECT_EQ(j1.con.redis[1].host, "2.1.1.2");
    EXPECT_EQ(j1.con.redis[1].port, 200U);
}

const static string jstr("{\"_id\":1, \"b\":\"Hello\\\"\", \"s\":[{\"s1\":2, \"s2\":\"hahaha\"},{\"s1\":99, \"s2\":\"nani\"}]}");

TEST(json, str)
{
    JTest j1;
    X::loadjson(jstr, j1, false);
    EXPECT_EQ(j1.a, 1);
    EXPECT_TRUE(j1.xhas("a"));
    EXPECT_TRUE(j1.xhas("b"));
    EXPECT_EQ(j1.b, "Hello\"");
    EXPECT_EQ(j1.s.size(), 2U);
    EXPECT_EQ(j1.s[0].s1, 2);
    EXPECT_EQ(j1.s[0].s2, "hahaha");
    EXPECT_EQ(j1.s[1].s1, 99);
    EXPECT_EQ(j1.s[1].s2, "nani");

    JTest j2;
    X::loadjson("{\"_id\":9}", j2, false);
    EXPECT_EQ(j2.a, 9);
}

#ifndef NO_BSON
TEST(bson, str)
{
    JTest j1;
    mongo::BSONObj bobj = mongo::fromjson(jstr);
    B::loadbson(bobj, j1);
    EXPECT_EQ(j1.a, 1);
    EXPECT_TRUE(j1.xhas("a"));
    EXPECT_TRUE(j1.xhas("b"));
    EXPECT_TRUE(j1.xhas("s"));
    EXPECT_EQ(j1.b, "Hello\"");
    EXPECT_EQ(j1.s.size(), 2U);
    EXPECT_EQ(j1.s[0].s1, 2);
    EXPECT_EQ(j1.s[0].s2, "hahaha");
    EXPECT_EQ(j1.s[1].s1, 99);
    EXPECT_EQ(j1.s[1].s2, "nani");
    cout<<"BSON test done"<<endl;
}
#endif

TEST(json, file)
{
    JTest j1;
    JTest j2;
    X::loadjson("test1.json", j1, true);
    EXPECT_EQ(j1.a, 1);
    EXPECT_EQ(j1.b, "Hello\"");
    EXPECT_EQ(j1.s.size(), 2U);
    EXPECT_EQ(j1.s[0].s1, 2);
    EXPECT_EQ(j1.s[0].s2, "hahaha");
    EXPECT_EQ(j1.s[0].s3, 20);
    EXPECT_EQ(j1.s[1].s1, 99);
    EXPECT_EQ(j1.s[1].s2, "nani");
    EXPECT_EQ(j1.s[1].s3, 30);
    EXPECT_EQ(j1.con.redis.size(), 2U);
    EXPECT_EQ(j1.con.redis[0].host, "2.1.1.1");
    EXPECT_EQ(j1.con.redis[0].port, 200U);
    EXPECT_EQ(j1.con.redis[1].host, "2.1.1.2");
    EXPECT_EQ(j1.con.redis[1].port, 200U);
    EXPECT_EQ(j1.ha.a, 13);
    EXPECT_EQ(j1.ha.b, 14);
    EXPECT_EQ(j1.vi[0], 2016);
    cout<<"to string"<<endl<<X::tojson(j1, "");
}

TEST(xml, file)
{
    JTest j1;
    JTest j2;
    X::loadxml("test1.xml", j1, true);
    EXPECT_EQ(j1.a, 1);
    EXPECT_EQ(j1.b, "Hello");
    EXPECT_EQ(j1.s.size(), 2U);
    EXPECT_EQ(j1.s[0].s1, 2);
    EXPECT_EQ(j1.s[0].s2, "hahaha");
    EXPECT_EQ(j1.s[1].s1, 99);
    EXPECT_EQ(j1.s[1].s2, "nani");
    EXPECT_EQ(j1.con.redis.size(), 2U);
    EXPECT_EQ(j1.con.redis[0].host, "2.1.1.1");
    EXPECT_EQ(j1.con.redis[0].port, 200U);
    EXPECT_EQ(j1.con.redis[1].host, "2.1.1.2");
    EXPECT_EQ(j1.con.redis[1].port, 200U);
}


#ifndef USE_BLADE
int main(int argc, char *argv[])
{
    #undef TEST
    #define TEST(a,b) a##_##b()
    TEST(config, file);
    TEST(json, str);
    #ifndef NO_BSON
    TEST(bson, str);
    #endif
    TEST(json, file);
    TEST(xml, file);
}
#endif
