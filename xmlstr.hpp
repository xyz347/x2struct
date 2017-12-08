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
#include <string>
#include <map>
#include <vector>
#include <set>
#include <sstream>

#include "xstr.hpp"

namespace x2struct {

class XmlStr:public XStr {
public:
    XmlStr(bool newline=false, int space=4);
    ~XmlStr();
public:
    void begin(const std::string&root, int space);
    void end(const std::string&root, int space);
    int  space();
    std::string toStr() const;
public:
    virtual void convert(const std::string&name, const std::string& data, int space, int index);

    void convert(const std::string&name, int16_t data, int space, int index);
    void convert(const std::string&name, uint16_t data, int space, int index);
    void convert(const std::string&name, int32_t data, int space, int index);
    void convert(const std::string&name, uint32_t data, int space, int index);
    void convert(const std::string&name, int64_t data, int space, int index);
    void convert(const std::string&name, uint64_t data, int space, int index);
    void convert(const std::string&name, float data, int space, int index);
    void convert(const std::string&name, double data, int space, int index);
    void convert(const std::string&name, bool data, int space, int index);

    template <typename TYPE>
    void convert(const std::string&name, const std::vector<TYPE> &data, int space, int index) {
        for (size_t i=0; i<data.size(); ++i) {
            this->convert(name, data[i], space, (int)i);
        }
    }

    template <typename TYPE>
    void convert(const std::string&name, const std::set<TYPE> &data, int space, int index) {
        int i;
        for (typename std::set<TYPE>::const_iterator it=data.begin(); it!=data.end(); ++it) {
            this->convert(name, *it, space, i++);
        }
    }

    template <typename TYPE>
    void convert(const std::string&name, const TYPE& data, int space, int index) {
        data.__struct_to_str(*this, name, space);
    }
private:
    template <typename TYPE>
    void base_convert(const std::string&name, const TYPE& data, int space) {
        if (_newline) {
            _ss<<std::string(space, ' ');
        }
        _ss<<'<'<<name<<'>'<<data<<"</"<<name<<'>';
        if (_newline) {
            _ss<<'\n';
        }
    }
private:
    bool _newline;
    int  _space;
    std::stringstream _ss;
};

}

