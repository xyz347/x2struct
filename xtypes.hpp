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


#pragma once

#include <stdint.h>
#include "xobj.hpp"

namespace x2struct {

class XType {
public:
    void __x_to_struct(XObj& obj) {
        std::string str;
        obj.convert(str);
        this->from_string(str);
    }
    bool __x_condition(XObj& obj, const std::string&name) {
        (void)obj;
        (void)name;
        return true;
    }
    template <class CLASS>
    void __struct_to_str(CLASS& obj, const std::string&name, int splen) const {
        std::string str = this->to_string();
        obj.convert(name, str, splen, 0);
    }
public:
    virtual std::string to_string() const = 0;
    virtual void from_string(const std::string&str) = 0;
};

// YYYY-MM-DD HH:MM:SS
class XDate:public XType{
public:
    int64_t unix_time;   // unix time
public:
    virtual std::string to_string() const;
    virtual void from_string(const std::string&str);
};

}

