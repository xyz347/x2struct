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

#ifndef __X_TYPES_H
#define __X_TYPES_H

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#define __X_DEF_XOPEN
#endif

#include <time.h>
#include <stdint.h>
#include <string>
#include <stdexcept>

#ifdef __X_DEF_XOPEN
#undef _XOPEN_SOURCE
#endif

namespace x2struct {

template<typename TYPE>
class XType {
public:
    template<class DOC>
    void __x_to_struct(DOC& obj) {
        std::string str;
        obj.convert(str);
        _t.parse(str);
    }
    template<class DOC>
    bool __x_condition(DOC& obj, const std::string&name) {
        (void)obj;
        (void)name;
        return true;
    }
    template<class DOC>
    void __struct_to_str(DOC& obj, const char*key) const {
        std::string str = _t.format();
        obj.convert(key, str);
    }
    TYPE* operator->() {
        return &_t;
    }
private:
    TYPE _t;
};

// YYYY-MM-DD HH:MM:SS
class _XDate {
public:
    int64_t unix_time;   // unix time
public:
    std::string format() const {
        time_t tt = (time_t)unix_time;
        tm     ttm;

        #ifndef WINNT
        localtime_r(&tt, &ttm);
        #else
        localtime_s(&ttm, &tt);
        #endif
        char buf[64];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ttm);
        return buf;
    }
    void parse(const std::string&str) {
        #ifndef WINNT
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
};

typedef XType<_XDate> XDate;

}

#endif
