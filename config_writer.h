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

#ifndef __X_CONFIG_WRITER_H
#define __X_CONFIG_WRITER_H


#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <string.h>

#include "thirdparty/libconfig/include/libconfig.h++"

#include "util.h"
#include "xtypes.h"
#include "traits.h"

#define LIBCONFIG_BUFFER_SIZE 1024
#define LIBCONFIG_TYPE_OBJECT 0
#define LIBCONFIG_TYPE_ARRAY  1

namespace x2struct {

class ConfigWriter {
public:
    ConfigWriter(int indentCount=0, char indentChar=' '):_indentCount(indentCount),_indentChar(indentChar) {
        if (_indentCount > 0) {
            if (_indentChar!=' ' && _indentChar!='\t') {
                throw std::runtime_error("indentChar must be space or tab");
            }
        }
        _buffer.resize(1);
        _buffer.reserve(LIBCONFIG_BUFFER_SIZE);
        _cur = &_buffer[0];
        _need_sep = false;
    }
    ~ConfigWriter() {
    }
public:
    std::string toStr() {
        size_t total_len = 0;
        for (size_t i=0; i<_buffer.size(); ++i) {
            total_len += _buffer[i].length();
        }
        std::string buf;
        buf.reserve(total_len);
        for (size_t i=0; i<_buffer.size(); ++i) {
            buf.append(_buffer[i]);
        }
        return buf;
    }

    void x2struct_set_key(const char*key){ // openssl defined set_key macro ...
        if (0!=key && key[0]!='\0') {
            append(key, -1);
            if (_indentCount <= 0) {
                append("=", 1);
            } else {
                append(" = ", 3);
            }
        }
    }
    void array_begin() {
        _need_sep = false;
        append("(", 1);
        _state.push_back(LIBCONFIG_TYPE_ARRAY);
    }
    void array_end() {
        _need_sep = false;
        _state.pop_back();
        indent();
        append(");", 1);
    }
    void object_begin() {
        _need_sep = false;
        append("{", 1);
        _state.push_back(LIBCONFIG_TYPE_OBJECT);
    }
    void object_end() {
        _need_sep = false;
        _state.pop_back();
        indent();
        append("};", 1);
    }
    const std::string&type() {
        static std::string t("config");
        return t;
    }

    ConfigWriter& convert(const char*key, const std::string &val) {
        indent();
        x2struct_set_key(key);

        append("\"", 1);
        for (size_t i=0; i<val.length(); ++i) {
            int c = (int)(val[i]) & 0xFF;
            switch (c) {
              case '\"':
              case '\\':
                append("\\", 1);
                append(val[i]);
                break;

              case '\n':
                append("\\n", 2);
                break;

              case '\r':
                append("\\r", 2);
                break;

              case '\f':
                append("\\f", 2);
                break;

              case '\t':
                append("\\t", 2);
                break;

              default:
                if(c >= ' ') {
                    append(val[i]);
                } else {
                    char tmp[16];
                    int l = snprintf(tmp, sizeof(tmp)-1, "\\x%02X", c);
                    append(tmp, l);
                }
            }
        }

        append("\"", 1);

        return *this;
    }
    #ifdef XTOSTRUCT_SUPPORT_CHAR_ARRAY
    ConfigWriter& convert(const char*key, const char val[]) {
        std::string str(val);
        return this->convert(key, str);
    }
    #endif
    ConfigWriter& convert(const char*key, bool val) {
        indent();
        x2struct_set_key(key);
        if (val) {
            append("true", 4);
        } else {
            append("false", 5);
        }
        return *this;
    }
    ConfigWriter& convert(const char*key, int16_t val) {
        indent();
        x2struct_set_key(key);
        append(Util::tostr(val));
        return *this;
    }
    ConfigWriter& convert(const char*key, uint16_t val) {
        indent();
        x2struct_set_key(key);
        append(Util::tostr(val));
        return *this;
    }
    ConfigWriter& convert(const char*key, int32_t val) {
        indent();
        x2struct_set_key(key);
        append(Util::tostr(val));
        return *this;
    }
    ConfigWriter& convert(const char*key, uint32_t val) {
        indent();
        x2struct_set_key(key);
        append(Util::tostr(val));
        return *this;
    }
    ConfigWriter& convert(const char*key, int64_t val) {
        indent();
        x2struct_set_key(key);
        append(Util::tostr(val).append("L"));
        return *this;
    }
    ConfigWriter& convert(const char*key, uint64_t val) {
        indent();
        x2struct_set_key(key);
        append(Util::tostr(val).append("L"));
        return *this;
    }
    ConfigWriter& convert(const char*key, double val) {
        indent();
        x2struct_set_key(key);
        append(Util::tostr(val));
        return *this;
    }
    ConfigWriter& convert(const char*key, float val) {
        indent();
        x2struct_set_key(key);
        append(Util::tostr(val));
        return *this;
    }

    template<typename T>
    ConfigWriter& convert(const char*key, const std::vector<T>&data) {
        indent();
        x2struct_set_key(key);
        this->array_begin();
        for (size_t i=0; i<data.size(); ++i) {
            this->convert("", data[i]);
        }
        this->array_end();
        return *this;
    }

    template<typename T>
    ConfigWriter& convert(const char*key, const std::list<T>&data) {
        indent();
        x2struct_set_key(key);
        this->array_begin();
        for (typename std::list<T>::const_iterator it=data.begin(); it!=data.end(); ++it) {
            this->convert("", *it);
        }
        this->array_end();
        return *this;
    }

    template<typename T>
    ConfigWriter& convert(const char*key, const std::set<T>&data) {
        indent();
        x2struct_set_key(key);
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
        x2struct_set_key(key);
        this->object_begin();
        for (typename std::map<std::string,T>::const_iterator iter=data.begin(); iter!=data.end(); ++iter) {
            this->convert(iter->first.c_str(), iter->second);
        }
        this->object_end();
    }

    template <typename KEY, typename T>
    void convert(const char*key, const std::map<KEY, T> &data) {
        indent();
        x2struct_set_key(key);
        this->object_begin();
        for (typename std::map<KEY,T>::const_iterator iter=data.begin(); iter!=data.end(); ++iter) {
            std::string _k("x");
            _k.append(Util::tostr(iter->first));
            this->convert(_k.c_str(), iter->second);
        }
        this->object_end();
    }


    #ifdef X_SUPPORT_C0X
    // unordered_map
    template <typename T>
    void convert(const char*key, const std::unordered_map<std::string, T> &data) {
        indent();
        x2struct_set_key(key);
        this->object_begin();
        for (typename std::unordered_map<std::string,T>::const_iterator iter=data.begin(); iter!=data.end(); ++iter) {
            this->convert(iter->first.c_str(), iter->second);
        }
        this->object_end();
    }

    // shared_ptr
    template <typename TYPE>
    void convert(const char*key, const std::shared_ptr<TYPE>& val) {
        if (NULL == val.get()) {
            return;
        }
        this->convert(key, *val);
    }

    // class/struct that not defined macro XTOSTRUCT
    template <typename T, typename std::enable_if<!x_has_x2struct<T>::value, int>::type = 0>
    void convert(const char*key, const T& data, x_for_class(T, int) *unused=0) {
        (void)unused;
        indent();
        x2struct_set_key(key);
        this->object_begin();
        x_struct_to_str(*this, key, data);
        this->object_end();
    }

    // class/struct that defined macro XTOSTRUCT
    template <typename T, typename std::enable_if<x_has_x2struct<T>::value, int>::type = 0>
    void convert(const char*key, const T& data) {
    #else
    template <typename T>
    void convert(const char*key, const T& data, x_for_class(T, int) *p=0) {
        (void)p;
    #endif
        indent();
        x2struct_set_key(key);
        this->object_begin();
        data.__struct_to_str(*this, key);
        this->object_end();
    }

    template <typename T>
    void convert(const char*key, const XType<T>& data) {
        data.__struct_to_str(*this, key);
    }

private:
    void append(const char* str, int len) {
        if (len < 0) {
            len = strlen(str);
        }
        if (len+_cur->length() <= LIBCONFIG_BUFFER_SIZE) {
            _cur->append(str, len);
        } else {
            _buffer.push_back(std::string());
            _cur = &_buffer[_buffer.size()-1];
            _cur->reserve(LIBCONFIG_BUFFER_SIZE);
            _cur->append(str, len);
        }
    }
    void append(const std::string&str) {
        append(str.c_str(), str.length());
    }
    void append(char ch) {
        char buf[1] = {ch};
        append(buf, 1);
    }
    void indent() {
        if (_need_sep) {
            if (_state.size()==0 || _state[_state.size()-1]==LIBCONFIG_TYPE_OBJECT) {
                append(";", 1);
            } else {
                append(",", 1);
            }
        }
        _need_sep = true;
        if (_indentCount>=0 && _state.size()>0) {
            append("\n", 1);
        }
        if (_indentCount>0) {
            append(std::string(_indentCount*_state.size(), _indentChar));
        }
    }

    int  _indentCount;
    char _indentChar;
    bool _need_sep;             // 是否需要分隔符
    std::vector<std::string> _buffer;
    std::string *_cur;
    std::vector<int> _state;
};

}

#endif
