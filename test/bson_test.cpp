/*
* Copyright (C) 2018 YY Inc. All rights reserved.
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


// included by x2struct_test.cpp

struct BSTest {
    int64_t id;
    map<string, int> dmap;
    vector<vector<int> > vi;
    vector<string> vs;
    XTOSTRUCT(A(id,"_id"), O(dmap, vi, vs));
};

TEST(bson, obj)
{
    string json = "{\"_id\":123, \"dmap\":{\"hello\":1, \"good\":2}, \"vi\":[[3,4],[13,14]], \"vs\":[\"5\", \"6\"]}";

    BSTest n;
    bson_error_t err;
    memset(&err, 0, sizeof(err));
    bson_t * bson = bson_new_from_json((const uint8_t *)json.data(), json.length(), &err);
    x2struct::X::loadbson(bson_get_data(bson), 0, n);
    bson_destroy(bson);

    EXPECT_EQ(n.id, 123);
    EXPECT_EQ(n.dmap["hello"], 1);
    EXPECT_EQ(n.dmap["good"], 2);
    EXPECT_EQ(n.vi[0][0], 3);
    EXPECT_EQ(n.vi[0][1], 4);
    EXPECT_EQ(n.vi[1][0], 13);
    EXPECT_EQ(n.vi[1][1], 14);
    EXPECT_EQ(n.vs[0], "5");
    EXPECT_EQ(n.vs[1], "6");
}

TEST(bson, bidir)
{
    BSTest st;
    st.id=123;
    st.dmap["hello"] = 1;
    st.dmap["good"] = 2;
    st.vi.resize(2);
    st.vi[0].push_back(3);
    st.vi[0].push_back(4);
    st.vi[1].push_back(13);
    st.vi[01].push_back(14);
    st.vs.push_back("5");
    st.vs.push_back("6");
    
    string bson = x2struct::X::tobson(st);
    BSTest n;
    x2struct::X::loadbson((uint8_t*)bson.data(), bson.length(), n);
    EXPECT_EQ(n.id, 123);
    EXPECT_EQ(n.dmap["hello"], 1);
    EXPECT_EQ(n.dmap["good"], 2);
    EXPECT_EQ(n.vi[0][0], 3);
    EXPECT_EQ(n.vi[0][1], 4);
    EXPECT_EQ(n.vi[1][0], 13);
    EXPECT_EQ(n.vi[1][1], 14);
    EXPECT_EQ(n.vs[0], "5");
    EXPECT_EQ(n.vs[1], "6");
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

TEST(bson, convert)
{
    BsonStr sub = BsonStr().convert("hello", 1).convert("good.abc.1", "nice");
    std::string str = BsonStr().convert("$set", sub).json();
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
