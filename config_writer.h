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

#include <string>
#include <vector>
#include <set>
#include <map>

#include "xtypes.h"

namespace x2struct {

class ConfigWriter {
public:
    ConfigWriter(int identCount=0, char identChar=' ');
    ~ConfigWriter();
public:
    std::string toStr();

    void set_key(const char*key);
    void array_begin();
    void array_end();
    void object_begin();
    void object_end();
    const std::string&type();

    ConfigWriter& convert(const char*key, const std::string &val);
    ConfigWriter& convert(const char*key, bool val);
    ConfigWriter& convert(const char*key, int16_t val);
    ConfigWriter& convert(const char*key, uint16_t val);
    ConfigWriter& convert(const char*key, int32_t val);
    ConfigWriter& convert(const char*key, uint32_t val);
    ConfigWriter& convert(const char*key, int64_t val);
    ConfigWriter& convert(const char*key, uint64_t val);
    ConfigWriter& convert(const char*key, double val);
    ConfigWriter& convert(const char*key, float val);

    template<typename T>
    ConfigWriter& convert(const char*key, const std::vector<T>&data) {
        indent();
        set_key(key);
        this->array_begin();
        for (size_t i=0; i<data.size(); ++i) {
            this->convert("", data[i]);
        }
        this->array_end();
        return *this;
    }

    template<typename T>
    ConfigWriter& convert(const char*key, const std::set<T>&data) {
        indent();
        set_key(key);
        this->array_begin();
        for (typename std::set<T>::const_iterator it=data.begin(); it!=data.end(); ++it) {
            this->convert("", *it);
        }
        this->array_end();
        return *this;
    }

    template <typename T>
    void convert(const char*key, const std::map<std::string, T> &data) {
        indent();
        set_key(key);
        this->object_begin();
        for (typename std::map<std::string,T>::const_iterator iter=data.begin(); iter!=data.end(); ++iter) {
            this->convert(iter->first, iter->second);
        }
        this->object_end();
    }

    template <typename KEY, typename T>
    void convert(const char*key, const std::map<KEY, T> &data) {
        indent();
        set_key(key);
        this->object_begin();
        for (typename std::map<KEY,T>::const_iterator iter=data.begin(); iter!=data.end(); ++iter) {
            std::string _k("x");
            _k.append(tostr(iter->first));
            this->convert(_k.c_str(), iter->second);
        }
        this->object_end();
    }

    template <typename T>
    void convert(const char*key, const T& data) {
        indent();
        set_key(key);
        this->object_begin();
        data.__struct_to_str(*this, key);
        this->object_end();
    }

    template <typename T>
    void convert(const char*key, const XType<T>& data) {
        data.__struct_to_str(*this, key);
    }

private:
    void append(const char* str, int len);
    void append(const std::string&str);
    void append(char ch);
    void indent();
    int  _indentCount;
    char _indentChar;
    bool _need_sep;             // 是否需要分隔符
    std::vector<std::string> _buffer;
    std::string *_cur;
    std::vector<int> _state;
};

}


