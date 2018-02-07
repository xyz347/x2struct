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

#include <string.h>
#include <libconfig.h++>
#include <iostream>

#include "util.h"
#include "config_writer.h"

namespace x2struct {

using namespace ::std;

#define BUFFER_SIZE 1024
#define TYPE_OBJECT 0
#define TYPE_ARRAY  1

ConfigWriter::ConfigWriter(int indentCount, char indentChar):_indentCount(indentCount),_indentChar(indentChar)
{
    if (_indentCount > 0) {
        if (_indentChar!=' ' && _indentChar!='\t') {
            throw runtime_error("indentChar must be space or tab");
        }
    }
    _buffer.resize(1);
    _buffer.reserve(BUFFER_SIZE);
    _cur = &_buffer[0];
    _need_sep = false;
}

ConfigWriter::~ConfigWriter()
{
}

// copy from libconfig.c
ConfigWriter& ConfigWriter::convert(const char*key, const std::string& val)
{
    indent();
    set_key(key);

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
                int l = sprintf(tmp, "\\x%02X", c);
                append(tmp, l);
            }
        }
    }

    append("\"", 1);

    return *this;
}

ConfigWriter& ConfigWriter::convert(const char*key, bool val)
{
    indent();
    set_key(key);
    if (val) {
        append("true", 4);
    } else {
        append("false", 5);
    }
    return *this;
}

ConfigWriter& ConfigWriter::convert(const char*key, int16_t val)
{
    indent();
    set_key(key);
    append(tostr(val));
    return *this;
}

ConfigWriter& ConfigWriter::convert(const char*key, uint16_t val)
{
    indent();
    set_key(key);
    append(tostr(val));
    return *this;
}

ConfigWriter& ConfigWriter::convert(const char*key, int32_t val)
{
    indent();
    set_key(key);
    append(tostr(val));
    return *this;
}

ConfigWriter& ConfigWriter::convert(const char*key, uint32_t val)
{
    indent();
    set_key(key);
    append(tostr(val));
    return *this;
}

ConfigWriter& ConfigWriter::convert(const char*key, int64_t val)
{
    indent();
    set_key(key);
    append(tostr(val).append("L"));
    return *this;
}

ConfigWriter& ConfigWriter::convert(const char*key, uint64_t val)
{
    indent();
    set_key(key);
    append(tostr(val).append("L"));
    return *this;
}

ConfigWriter& ConfigWriter::convert(const char*key, double val)
{
    indent();
    set_key(key);
    append(tostr(val));
    return *this;
}

ConfigWriter& ConfigWriter::convert(const char*key, float val)
{
    indent();
    set_key(key);
    append(tostr(val));
    return *this;
}

std::string ConfigWriter::toStr()
{
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

void ConfigWriter::set_key(const char*key)
{
    if (0!=key && key[0]!='\0') {
        append(key, -1);
        if (_indentCount <= 0) {
            append("=", 1);
        } else {
            append(" = ", 3);
        }
    }
}
void ConfigWriter::array_begin()
{
    _need_sep = false;
    append("(", 1);
    _state.push_back(TYPE_ARRAY);
}
void ConfigWriter::array_end()
{
    _need_sep = false;
    _state.pop_back();
    indent();
    append(");", 1);
}
void ConfigWriter::object_begin()
{
    _need_sep = false;
    append("{", 1);
    _state.push_back(TYPE_OBJECT);
}
void ConfigWriter::object_end()
{
    _need_sep = false;
    _state.pop_back();
    indent();
    append("};", 1);
}

void ConfigWriter::indent() 
{
    if (_need_sep) {
        if (_state.size()==0 || _state[_state.size()-1]==TYPE_OBJECT) {
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
        append(string(_indentCount*_state.size(), _indentChar));
    }
}

const std::string& ConfigWriter::type()
{
    static string t("config");
    return t;
}


void ConfigWriter::append(const char* str, int len)
{
    if (len < 0) {
        len = strlen(str);
    }
    if (len+_cur->length() <= BUFFER_SIZE) {
        _cur->append(str, len);
    } else {
        _buffer.push_back(string());
        _cur = &_buffer[_buffer.size()-1];
        _cur->reserve(BUFFER_SIZE);
        _cur->append(str, len);
    }
}
void ConfigWriter::append(const std::string&str)
{
    append(str.c_str(), str.length());
}
void ConfigWriter::append(char ch)
{
    char buf[1] = {ch};
    append(buf, 1);
}

}
