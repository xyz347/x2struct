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


#include <iostream>
#include <fstream>
#include <string.h>

#include "example.h"
#include "bson_builder.h"

using namespace std;
using namespace x2struct;

int main(int argc, char *argv[]) {
    // struct <--> string
    {
        bson_error_t err;
        memset(&err, 0, sizeof(err));
        std::ifstream fs("test.json", std::ifstream::binary);
        std::string json((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
        bson_t * bson = bson_new_from_json((const uint8_t *)json.data(), json.length(), &err);
        cout<<"======== struct <-----> string ==========="<<endl;
        xstruct x;
        X::loadbson(bson_get_data(bson), 0, x);
        cout<<X::tojson(x)<<endl<<endl;             // yes, tojson
    }

    {
        cout<<"======== builder ==========="<<endl;
        std::vector<std::string> vstr;
        vstr.push_back("s1");
        vstr.push_back("s2");
        #if __GXX_EXPERIMENTAL_CXX0X__  // if support c++11 build map by initializer_list
        bb::vp m{{"$set", bb::vp{{"_id",200}, {"date",bb::dt(1512828045000)}, {"vs", vstr}}}};
        bb::vp m1{{"_id", bb::vp{{"$in", vector<string>{"a"}}}}};
        #else
        bb::vp m;
        bb::vp up;
        up.push_back(std::make_pair<std::string, bb::intf>("_id", 200));
        up.push_back(std::make_pair<std::string, bb::intf>("date", bb::dt(1512828045000)));
        up.push_back(std::make_pair<std::string, bb::intf>("vs", vstr));
        m.push_back(std::make_pair<std::string, bb::intf>("$set", up));
        #endif
        cout<<bb::json(m, false)<<endl<<endl;
    }

    {
        cout<<"======== writer ==========="<<endl;
        BsonWriter sub = BsonWriter().convert("hello", 1).convert("good.abc.1", "nice");
        cout<<BsonWriter().convert("$set", sub).json()<<endl<<endl;
    }

    {
        cout<<"======== raw ==========="<<endl;
        map<string, int> m;
        m["hello"] = 1;
        m["good"] = 2;
        string bson = x2struct::X::tobson(m);

        map<string, int> n;
        x2struct::X::loadbson((uint8_t*)bson.data(), bson.length(), n);
        cout<<X::tojson(n)<<endl<<endl;
    }
}