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


#include <time.h>

#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp> 

#include "xstr.hpp"

namespace x2struct {

using namespace ::std;

std::string XStr::fieldName(const std::string&oname, const std::string&aname)
{
    string nname;

    vector<string> types;
    boost::split(types, aname, boost::is_any_of(" "));
    for (size_t i=0; i<types.size(); ++i) {
        vector<string> tname;
        boost::split(tname, types[i], boost::is_any_of(":"));
        if (tname.size()==2 && tname[0]==_type) {
            nname = tname[1];
            break;
        } else if (tname.size()==1) {
            nname = types[i];
        }
    }

    if (!nname.empty()) {
        vector<string> tname;
        boost::split(tname, nname, boost::is_any_of(","));
        return tname[0];
    } else {
        return oname;
    }
}


}

