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



#include "xstdint.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>

#include <config.h>
#include "gtest_stub.h"

#define XTOSTRUCT_XML
#ifndef WINDOWS
#define XTOSTRUCT_LIBCONFIG
#define XTOSTRUCT_BSON
#include "bson_builder.h"
#endif

#include "example.h"

static void base_check(xstruct&x)
{
    int i;
    EXPECT_EQ(x.id, 100);

    EXPECT_EQ(x.start.unix_time, 1218196800);

    EXPECT_EQ(x.tint, 101);

    EXPECT_EQ(x.tstring, "hello\"");
    EXPECT_TRUE(0==strcmp(x.chArray, "It's Array"));

    #ifdef X_SUPPORT_C0X
    EXPECT_EQ(x.sp->a, 123);
    #else
    EXPECT_EQ(x.sp.a, 123);
    #endif

    EXPECT_EQ(x.umap.size(), 2U);
    EXPECT_EQ(x.umap["a"], 100);
    EXPECT_EQ(x.umap["b"], 101);

    EXPECT_EQ(x.vint.size(), 1U);
    EXPECT_EQ(x.vint[0], 102);

    EXPECT_EQ(x.lint.size(), 2U);
    i=0;
    int lint[] = {12,34};
    for (std::list<int>::const_iterator iter=x.lint.begin(); iter!=x.lint.end(); ++iter,++i) {
        EXPECT_EQ(*iter, lint[i]);
    }

    EXPECT_EQ(x.vstring.size(), 2U);
    EXPECT_EQ(x.vstring[0], "hello1");
    EXPECT_EQ(x.vstring[1], "hello2");

    EXPECT_EQ(x.vlong.size(), 2U);
    EXPECT_EQ(x.vlong[0], 666);
    EXPECT_EQ(x.vlong[1], 999);

    EXPECT_EQ(x.vsub.size(), 1U);
    EXPECT_EQ(x.vsub[0].a, 103);
    EXPECT_EQ(x.vsub[0].b, "hello3");

    EXPECT_EQ(x.vvint.size(), 1U);
    EXPECT_EQ(x.vvint[0].size(), 2U);
    EXPECT_EQ(x.vvint[0][0], -104);
    EXPECT_EQ(x.vvint[0][1], 105);

    EXPECT_EQ(x.vvstring.size(), 2U);
    EXPECT_EQ(x.vvstring[0].size(), 1U);
    EXPECT_EQ(x.vvstring[1].size(), 2U);
    EXPECT_EQ(x.vvstring[0][0], "hello4");
    EXPECT_EQ(x.vvstring[1][0], "hello5");
    EXPECT_EQ(x.vvstring[1][1], "hello6");

    EXPECT_EQ(x.vvsub.size(), 2U);
    EXPECT_EQ(x.vvsub[0].size(), 1U);
    EXPECT_EQ(x.vvsub[1].size(), 2U);
    EXPECT_EQ(x.vvsub[0][0].a, 105);
    EXPECT_EQ(x.vvsub[0][0].b, "hello7");
    EXPECT_EQ(x.vvsub[1][0].a, 106);
    EXPECT_EQ(x.vvsub[1][0].b, "hello8");
    EXPECT_EQ(x.vvsub[1][1].a, 107);
    EXPECT_EQ(x.vvsub[1][1].b, "hello9");

    EXPECT_EQ(x.tmap.size(), 2U);
    EXPECT_EQ(x.tmap[108].b, "hello10");
    EXPECT_EQ(x.tmap[109].b, "hello11");

    EXPECT_EQ(x.con.url, "hello12");

    EXPECT_EQ(x.md, MASTER);
}

TEST(json, unmarshal)
{
    xstruct x;
    X::loadjson("test.json", x, true);
    base_check(x);
}

TEST(json, map)
{
    string jstr("{\"a\":1, \"b\":2}");
    map<string, int> m;
    X::loadjson(jstr, m, false);
    EXPECT_EQ(m.size(), 2U);
    EXPECT_EQ(m["a"], 1);
    EXPECT_EQ(m["b"], 2);
}

TEST(json, marshal)
{
    xstruct x;
    X::loadjson("test.json", x, true);
    string n = X::tojson(x);
    cout<<n<<endl;

    xstruct y;
    X::loadjson(n, y, false);
    base_check(y);
}

TEST(json, invalid)
{
    string data("hello");
    map<string,string> m;

    bool excpt = false;
    try {
        X::loadjson(data, m, false);
    } catch (...) {
        excpt = true;
    }
    EXPECT_TRUE(excpt);
}

#ifdef XNT
TEST(notemplate, json_xml)
{
    struct LocalClass {
        int a;
        int b;
        int c;
        int d;
        XTOSTRUCT_NT(Json, Xml)(O(a,b), M(c,d));
    };

    LocalClass a;
    LocalClass b;
    LocalClass c;

    a.a = 1;b.a=0;c.a=0;
    a.b = 2;b.b=0;c.b=0;
    a.c = 3;b.c=0;c.c=0;
    a.d = 4;b.d=0;c.d=0;

    string s = x2struct::X::tojson(a);
    x2struct::X::loadjson(s, b, false);
    EXPECT_EQ(b.a, 1);
    EXPECT_EQ(b.b, 2);
    EXPECT_EQ(b.c, 3);
    EXPECT_EQ(b.d, 4);

    s = x2struct::X::toxml(a, "config");
    x2struct::X::loadxml(s, c, false);
    EXPECT_EQ(b.a, 1);
    EXPECT_EQ(b.b, 2);
    EXPECT_EQ(b.c, 3);
    EXPECT_EQ(b.d, 4);
}
#endif

#ifdef XTOSTRUCT_LIBCONFIG
TEST(config, unmarshal)
{
    xstruct x;
    X::loadconfig("test.cfg", x, true);
    base_check(x);
}

TEST(config, marshal)
{
    xstruct x;
    X::loadconfig("test.cfg", x, true);
    string n = X::toconfig(x, "root", 1, '\t');

    xstruct y;
    X::loadconfig(n, y, false);
    base_check(y);
}
#endif

#ifdef XTOSTRUCT_XML
TEST(xml, unmarshal)
{
    xstruct x;
    X::loadxml("test.xml", x, true);
    base_check(x);
}


TEST(xml, marshal)
{
    xstruct x;
    X::loadxml("test.xml", x, true);
    string n = X::toxml(x, "xmlroot");

    xstruct y;
    X::loadxml(n, y, false);
    base_check(y);
}
#endif

#ifdef XTOSTRUCT_BSON
TEST(bson, unmarshal)
{
    bson_error_t err;
    memset(&err, 0, sizeof(err));
    std::ifstream fs("test.json", std::ifstream::binary);
    std::string json((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
    bson_t * bson = bson_new_from_json((const uint8_t *)json.data(), json.length(), &err);
    xstruct x;
    X::loadbson(bson_get_data(bson), 0, x);
    bson_destroy(bson);

    base_check(x);
}

TEST(bson, marshal)
{
    bson_error_t err;
    memset(&err, 0, sizeof(err));
    std::ifstream fs("test.json", std::ifstream::binary);
    std::string json((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
    bson_t * bson = bson_new_from_json((const uint8_t *)json.data(), json.length(), &err);
    xstruct x;
    X::loadbson(bson_get_data(bson), 0, x);
    bson_destroy(bson);

    std::string n = X::tobson(x);
    xstruct y;
    X::loadbson(n, y, false);
    base_check(y);
}


TEST(bson, builder)
{
    std::vector<std::string> vstr;
    vstr.push_back("s1");
    vstr.push_back("s2");
    #if __GXX_EXPERIMENTAL_CXX0X__  // if support c++11 build map by initializer_list
    bb::vp m{{"$set", bb::vp{{"_id",200}, {"date",bb::dt(1512828045000)}, {"vs", vstr}}}};
    bb::vp m1{{"_id", bb::vp{{"$in", vector<string>{"a"}}}}};
    //EXPECT_EQ(bb::json(m1,false), "{\"_id\":{\"$in\":[\"a\"]}}");
    #else
    bb::vp m;
    bb::vp up;
    up.push_back(std::make_pair<std::string, bb::intf>("_id", 200));
    up.push_back(std::make_pair<std::string, bb::intf>("date", bb::dt(1512828045000)));
    up.push_back(std::make_pair<std::string, bb::intf>("vs", vstr));
    m.push_back(std::make_pair<std::string, bb::intf>("$set", up));
    cout<<bb::json(up, false)<<endl;
    #endif

    EXPECT_EQ(bb::json(m, false), "{\"$set\":{\"_id\":200,\"date\":1512828045000,\"vs\":[\"s1\",\"s2\"]}}");
}

TEST(bson, writer)
{
    BsonWriter sub = BsonWriter().convert("hello", 1).convert("good.abc.1", "nice");
    std::string str = BsonWriter().convert("$set", sub).json();
    EXPECT_EQ(str, "{ \"$set\" : { \"hello\" : 1, \"good.abc.1\" : \"nice\" } }");
}

TEST(bson, raw)
{
    map<string, int> m;
    m["hello"] = 1;
    m["good"] = 2;
    string bson = x2struct::X::tobson(m);

    map<string, int> n;
    x2struct::X::loadbson((uint8_t*)bson.data(), bson.length(), n);
    EXPECT_EQ(n["hello"], 1);
    EXPECT_EQ(n["good"], 2);
}
#endif

namespace {

string m_array;
JsonReader *m_obj;
TEST(performance, build_str)
{
    m_array.reserve(10240);
    m_array.append("[1");
    for (int i=2; i<=1024; ++i) {
        m_array.append(",");
        m_array.append(Util::tostr(i));
    }
    m_array.append("]");
}

TEST(performance, parse)
{
    m_obj = new JsonReader(m_array, false);
}

TEST(performance, convert)
{
    vector<int> m_v;
    m_obj->convert(NULL, m_v);
    EXPECT_EQ(m_v.size(), 1024U);
}

TEST(performance, clear)
{
    delete m_obj;
}

}

#ifdef XTOSTRUCT_GOCODE
TEST(gocode, test)
{
    xstruct x;
    cout<<x2struct::X::togocode(x, true, true, true)<<endl;
}
#endif


int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    const std::vector<text_ctx>& tcs = TC_CONTAINER::tcs();
    for (size_t i=0; i<tcs.size(); ++i) {
        #ifdef XTOSTRUCT_GOCODE
        if (tcs[i].group != "gocode") {
            continue;
        }
        #endif
        Status::c() = 0;
        cout<<tcs[i].group<<" "<<tcs[i].name<<" start --->"<<endl;
        tcs[i].tc();
        if (Status::c() == 0) {
            cout<<tcs[i].group<<" "<<tcs[i].name<<" passed.";
        } else {
            cout<<tcs[i].group<<" "<<tcs[i].name<<" fail!!!!!!";
        }
        cout<<"<---"<<endl;
    }
}

