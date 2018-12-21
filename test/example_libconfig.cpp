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

#include "example.h"


using namespace std;
using namespace x2struct;

int main(int argc, char *argv[]) {
    xstruct x;

    // struct <--> string
    cout<<"======== struct <-----> string ==========="<<endl;
    X::loadconfig("test.cfg", x, true);        // load libconfig from file, if from string, last parameter give false
    cout<<X::toconfig(x, "")<<endl<<endl;      // struct to string.

#if 0 // not support now
    // map <--> string
    cout<<"======== map <-----> string ==========="<<endl;
    string smap = "{\"a\":1, \"b\":2}";
    map<string, int> m;
    X::loadconfig(smap, m, false);
    cout<<X::toconfig(m, "")<<endl<<endl;

    // vector <--> string
    cout<<"======== vector <-----> string ==========="<<endl;
    string svector = "[1, 2, 3]";
    vector<int> v;
    X::loadconfig(svector, v, false);
    cout<<X::toconfig(v, "")<<endl<<endl;
#endif
}
