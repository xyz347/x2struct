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


#ifndef __X_TEST_EXAMPLE_H
#define __X_TEST_EXAMPLE_H

#define XTOSTRUCT_SUPPORT_CHAR_ARRAY
#include <x2struct.hpp>
//#include <xtypes.h>

using namespace std;
using namespace x2struct;

enum Mode {
    MASTER = 1,
    SLAVE = 2,
};

struct condition {
    int    id;
    string url;
    XTOSTRUCT(M(url));
};

#ifdef X_SUPPORT_C0X
struct sub {
    int    a;
    string b;
};
XTOSTRUCT_OUT(sub, M(a), O(b));
#else
struct sub {
    int    a;
    string b;
    XTOSTRUCT(M(a), O(b));
};
#endif

struct SharePtr {
    int a;
    XTOSTRUCT(O(a));
};

struct xstruct {
    int    id;
    XDate  start;
    int    tint;
    string tstring;
    char   chArray[16];
    #ifdef X_SUPPORT_C0X
    std::shared_ptr<SharePtr> sp;
    std::unordered_map<std::string, int> umap;
    #else
    SharePtr sp;
    std::map<std::string, int> umap;
    #endif
    vector<int> vint;
    list<int> lint;
    vector<string> vstring;
    vector<int64_t> vlong;
    vector<sub> vsub;
    vector<vector<int> > vvint;
    vector<vector<string> > vvstring;
    vector<vector<sub> > vvsub;
    map<int, sub> tmap;
    condition con;
    Mode md;
#ifndef XTOSTRUCT_GOCODE
    XTOSTRUCT(A(id,"config:id _id,m"),C(con), O(start, tint, tstring, chArray, sp, umap, vint, lint, vstring, vlong, vsub, vvint, vvstring, vvsub, tmap, md, con));
#else
    XTOSTRUCT(A(id,"config:id _id,m"),C(con), O(tint, tstring, sp, vint, vstring, vlong, vsub, vvint, vvstring, vvsub, tmap, con));
#endif
    XTOSTRUCT_CONDITION(xstruct, con) {
        int  _id;
        return obj.convert("id", _id)&&_id==this->id;
    }
};

#endif
