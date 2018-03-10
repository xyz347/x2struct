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
#include <iostream>

#include "util.h"
#include "xml_writer.h"

namespace x2struct {

using namespace ::std;

#define BUFFER_SIZE 1024
#define TYPE_OBJECT 0
#define TYPE_ARRAY  1


XmlWriter::XmlKey::XmlKey(const char*key, XmlWriter*writer, bool base)
{
    _key = key;
    if (_key[0]=='\0') {
        return;
    }
    _writer = writer;
    _base = base;
    _writer->indent();
    _writer->append("<", 1);
    _writer->append(_key, -1);
    _writer->append(">");
    _lines = _writer->_lines;
}

XmlWriter::XmlKey::~XmlKey()
{
    if (_key[0]=='\0') {
        return;
    }
    if (!_base || _writer->_lines>_lines) {
        _writer->indent();
    }
    _writer->append("</", 2);
    _writer->append(_key, -1);
    _writer->append(">");
}

XmlWriter::XmlWriter(int indentCount, char indentChar):_indentCount(indentCount),_indentChar(indentChar)
{
    if (_indentCount > 0) {
        if (_indentChar!=' ' && _indentChar!='\t') {
            throw runtime_error("indentChar must be space or tab");
        }
    }
    _buffer.resize(1);
    _buffer.reserve(BUFFER_SIZE);
    _cur = &_buffer[0];
    _lines = 0;
}

XmlWriter::~XmlWriter()
{
}

// 不知道完备没
XmlWriter& XmlWriter::convert(const char*key, const std::string& val)
{
    XmlKey xkey(key, this, true);

    for (size_t i=0; i<val.length(); ++i) {
        int c = (int)(val[i]) & 0xFF;
        switch (c) {
          case '<':
            append("&lt;", 4);
            break;
          case '>':
            append("&gt;", 4);
            break;
          case '&':
            append("&amp;", 5);
            break;
          case '\'':
            append("&apos;", 6);
            break;
          case '\"':
            append("&quot;", 6);
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

    return *this;
}

XmlWriter& XmlWriter::convert(const char*key, bool val)
{
    XmlKey xkey(key, this, true);
    if (val) {
        append("true", 4);
    } else {
        append("false", 5);
    }
    return *this;
}

XmlWriter& XmlWriter::convert(const char*key, int16_t val)
{
    XmlKey xkey(key, this, true);
    append(tostr(val));
    return *this;
}

XmlWriter& XmlWriter::convert(const char*key, uint16_t val)
{
    XmlKey xkey(key, this, true);
    append(tostr(val));
    return *this;
}

XmlWriter& XmlWriter::convert(const char*key, int32_t val)
{
    XmlKey xkey(key, this, true);
    append(tostr(val));
    return *this;
}

XmlWriter& XmlWriter::convert(const char*key, uint32_t val)
{
    XmlKey xkey(key, this, true);
    append(tostr(val));
    return *this;
}

XmlWriter& XmlWriter::convert(const char*key, int64_t val)
{
    XmlKey xkey(key, this, true);
    append(tostr(val));
    return *this;
}

XmlWriter& XmlWriter::convert(const char*key, uint64_t val)
{
    XmlKey xkey(key, this, true);
    append(tostr(val));
    return *this;
}

XmlWriter& XmlWriter::convert(const char*key, double val)
{
    XmlKey xkey(key, this, true);
    append(tostr(val));
    return *this;
}

XmlWriter& XmlWriter::convert(const char*key, float val)
{
    XmlKey xkey(key, this, true);
    append(tostr(val));
    return *this;
}

std::string XmlWriter::toStr()
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

void XmlWriter::array_begin()
{
    _state.push_back(TYPE_ARRAY);
}
void XmlWriter::array_end()
{
    _state.pop_back();
}
void XmlWriter::object_begin()
{
    _state.push_back(TYPE_OBJECT);
}
void XmlWriter::object_end()
{
    _state.pop_back();
}

void XmlWriter::indent() 
{
    if (_indentCount >= 0) {
        append("\n", 1);
        ++_lines;
    }
    size_t depth=_state.size();
    if (in_array()) {
        --depth;
    }
    if (_indentCount>0 && depth>0) {
        append(string(_indentCount*depth, _indentChar));
    }
}

const std::string& XmlWriter::type()
{
    static string t("xml");
    return t;
}


void XmlWriter::append(const char* str, int len)
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
void XmlWriter::append(const std::string&str)
{
    append(str.c_str(), str.length());
}
void XmlWriter::append(char ch)
{
    char buf[1] = {ch};
    append(buf, 1);
}

bool XmlWriter::in_array()
{
    return _state.size()>0&&_state[_state.size()-1]==TYPE_ARRAY;
}

}
