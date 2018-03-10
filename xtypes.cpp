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

#define _XOPEN_SOURCE
#include <time.h>
#include "xtypes.h"

namespace x2struct {

std::string _XDate::format() const
{
    time_t tt = (time_t)unix_time;
    tm     ttm;

    #ifndef WIN
    localtime_r(&tt, &ttm);
    #else
    localtime_s(&ttm, &tt);
    #endif
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ttm);
    return buf;
}

void _XDate::parse(const std::string&str)
{
    #ifndef WIN
    tm ttm;

    if (0 != strptime(str.c_str(), "%Y-%m-%d %H:%M:%S", &ttm)) {
        unix_time = mktime(&ttm);
    } else {
        std::string err("invalid time string[");
        err.append(str).append("]. use format YYYY-mm-dd H:M:S. ");
        throw std::runtime_error(err);
    }
    #else
    unix_time = 1218196800; // TODO for test only
    #endif
}

}


