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

#ifndef USE_MAKE
#include <gtest/gtest.h>
#include "test/blade_test_env.h"
#else
#include "test/gtest_stub.h"
#endif

#include <libbson-1.0/bson.h>
#include "x2struct.hpp"
#include "xtypes.hpp"
#include "bsonbuilder.hpp"


using namespace std;
using namespace x2struct;
using namespace bb;

#ifndef USE_MAKE
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
    XTOSTRUCT(M(a,b), O(c));
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
    map<int, haha> m;
    JTest(){start.unix_time = time(0);}
    XTOSTRUCT(A(a,"_id,me"),O(ha, start,con, b,s, vi, vs, m));
};
const static string jstr("{\"_id\":1, \"b\":\"Hello\\\"\", \"s\":[{\"s1\":2, \"s2\":\"hahaha\"},{\"s1\":99, \"s2\":\"nani\"}]} ");

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

    for (std::map<int,haha>::const_iterator iter=j1.m.begin(); iter!=j1.m.end(); ++iter) {
        cout<<iter->first<<':'<<iter->second.a<<','<<iter->second.b<<','<<iter->second.c<<endl;
    }
}


TEST(json, raw)
{
    vector<haha> data;
    X::loadjson("[{\"a\":1, \"b\":2, \"c\":3}, {\"a\":11, \"b\":22, \"c\":33}]", data, false);
    for (size_t i=0; i<data.size(); ++i) {
        cout<<data[i].a<<','<<data[i].b<<','<<data[i].c<<endl;
    }
}


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

    cout<<"to string"<<endl<<X::tojson(j1, "");
}


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
}

TEST(bson, obj)
{
    JTest j1;
    bson_error_t err;
    memset(&err, 0, sizeof(err));
    bson_t * bson = bson_new_from_json((const uint8_t *)jstr.data(), jstr.length(), &err);
    X::loadbson(bson_get_data(bson), 0, j1);

    // iter test begin ===========================
    #if 0
    bool iret;
    bson_iter_t iter;
    bson_iter_t sub;
    bson_iter_init(&iter, bson);
    iret = bson_iter_next(&iter);
    cout<<"ret:"<<iret<<bson_iter_key(&iter)<<endl;
    /*bson_iter_t sub;
    iret = bson_iter_recurse(&iter, &sub);
    bson_iter_next(&sub);
    cout<<"ret:"<<iret<<':'<<bson_iter_key(&sub)<<endl;*/
    #endif
    // iter test end==============================
    bson_destroy(bson);
    #if 1
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
    #endif
}

TEST(bson, objmap)
{
    std::string mstr("{\"1\":101, \"2\":202, \"3\":303}");
    std::map<int, int> m;
    bson_error_t err;
    memset(&err, 0, sizeof(err));
    bson_t * bson = bson_new_from_json((const uint8_t *)mstr.data(), mstr.length(), &err);
    X::loadbson(bson_get_data(bson), 0, m);
    for (map<int,int>::const_iterator iter = m.begin(); iter!=m.end(); ++iter) {
        cout<<iter->first<<':'<<iter->second<<endl;
    }
    bson_destroy(bson);
}

TEST(bson, builder)
{
    #if __GXX_EXPERIMENTAL_CXX0X__  // if support c++11 build map by initializer_list
    bb::mi m{{"$set", bb::mi{{"_id",200}, {"date",bb::dt(1512828045000)}}}};
    #else
    bb::mi up;
    bb::mi m;
    up.insert(std::make_pair<std::string, bb::intf>("_id", 200));
    up.insert(std::make_pair<std::string, bb::intf>("date", bb::dt(1512828045000)));
    m.insert(std::make_pair<std::string, bb::intf>("$set", up));
    #endif

    bson_t b;
    std::string data = build(m, &b);
    size_t len;
    char *json = bson_as_json(&b, &len);
    cout<<json<<endl;
    bson_free(json);
    bson_destroy(&b);
}

TEST(bson, convert)
{
    BsonStr sub = BsonStr().convert("hello", 1).convert("good.abc.1", "nice");
    std::string str = BsonStr().convert("$set", sub).toStr();
    bson_t b;
    bson_init_static(&b, (const uint8_t *)str.data(), str.length());
    size_t s;
    char *json = bson_as_json(&b, &s);
    cout<<"=========="<<json<<"================"<<endl;
    bson_free(json);
}

struct STR_SUB {
    int i;
    float f;
    string s;
    XTOSTRUCT(O(i,f,s));
};
struct STR {
    STR_SUB sub;
    vector<vector<int> > vvs;
    set<int> ss;
    map<int, STR_SUB> ms;
    XTOSTRUCT(O(sub, vvs,ss, ms));
};
TEST(bson, str)
{
    #if 1
    STR s;
    s.sub.i = 10;
    s.sub.f = 3.14;
    s.sub.s = "substring";
    s.vvs.resize(2);
    s.vvs[0].push_back(1);
    s.vvs[0].push_back(2);
    s.vvs[1].push_back(11);
    s.vvs[1].push_back(12);
    s.ss.insert(100);
    s.ss.insert(101);
    s.ms[200].i = 201;
    s.ms[200].f = 201.11;
    s.ms[200].s = "202";
    s.ms[400].i = 401;
    s.ms[400].f = 401.11;
    s.ms[400].s = "402";
    std::string bstr = X::tobson(s);
    bson_t b;
    bson_init_static(&b, (const uint8_t *)bstr.data(), bstr.length());

    size_t l;
    char *json = bson_as_json(&b, &l);
    cout<<json<<endl;
    bson_free(json);

    float f = tonum<float>("3.14");
    cout<<f<<endl;
    #endif

    XDate date;
    date.from_string("2017-12-08 16:00:00");
    cout<<"time:"<<date.unix_time<<endl;
    cout<<date.to_string()<<endl;
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


#ifdef USE_MAKE
int main(int argc, char *argv[])
{
    const std::vector<test_case>& tcs = TC_CONTAINER::tcs();
    for (size_t i=0; i<tcs.size(); ++i) {
        tcs[i]();
    }
}
#endif

