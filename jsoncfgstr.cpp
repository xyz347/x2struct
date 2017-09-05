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



#include <json/json.h>

#include "jsoncfgstr.hpp"

namespace x2struct {

using namespace std;


JsonCfgStr::JsonCfgStr(bool isjson, bool newline, int splen)
    :XStr(isjson?"json":"config"),_json(isjson), _newline(newline),_space(splen)
{
    if (isjson) {
        _vecb = "[";
        _vece = "]";
        _key = "\"";
        _assign = ":";
        _sep = ",";
    } else {
        _vecb = "(";
        _vece = ")";
        _assign = "=";
        _sep = ";";
    }
}

JsonCfgStr::~JsonCfgStr()
{
}

void JsonCfgStr::begin(const string&root, int splen)
{
    if (root.empty()) {
        _ss<<string(splen, ' ')<<"{";
    } else {
        _ss<<string(splen, ' ')<<_key<<root<<_key<<_assign<<"{";
    }
}

void JsonCfgStr::end(const string&root, int splen)
{
    if (_newline) {
        _ss<<'\n';
    }
    _ss<<string(splen, ' ')<<"}";
}

int  JsonCfgStr::space()
{
    return _newline?_space:0;
}

string JsonCfgStr::toStr()const 
{
    return _ss.str();
}

void JsonCfgStr::convert(const string&name, int16_t data, int splen, int index)
{
    base_convert(name, data, splen, index);
}

void JsonCfgStr::convert(const string&name, uint16_t data, int splen, int index)
{
    base_convert(name, data, splen, index);
}

void JsonCfgStr::convert(const string&name, int32_t data, int splen, int index)
{
    base_convert(name, data, splen, index);
}

void JsonCfgStr::convert(const string&name, uint32_t data, int splen, int index)
{
    base_convert(name, data, splen, index);
}

void JsonCfgStr::convert(const std::string&name, int64_t data, int splen, int index)
{
    base_convert(name, data, splen, index);
}

void JsonCfgStr::convert(const std::string&name, uint64_t data, int splen, int index)
{
    base_convert(name, data, splen, index);
}

void JsonCfgStr::convert(const std::string&name, float data, int splen, int index)
{
    base_convert(name, data, splen, index);
}

void JsonCfgStr::convert(const std::string&name, double data, int splen, int index)
{
    base_convert(name, data, splen, index);
}

void JsonCfgStr::convert(const std::string&name, bool data, int splen, int index)
{
    base_convert(name, data?"true":"false", splen, index);
}

void JsonCfgStr::convert(const string&name, const string& data, int splen, int index)
{
    linefeed(index);
    if (_json) {
        _ss<<string(splen, ' ')<<_key<<name<<_key<<_assign<<Json::valueToQuotedString(data.c_str());
    } else {
        _ss<<string(splen, ' ')<<_key<<name<<_key<<_assign<<'"'<<data<<'"';
    }
}


// base type vector
void JsonCfgStr::convert(const std::string&name, const std::vector<int16_t>& data, int splen, int index)
{
    base_vector(name, data, splen, index);
}

void JsonCfgStr::convert(const std::string&name, const std::vector<uint16_t>& data, int splen, int index)
{
    base_vector(name, data, splen, index);
}

void JsonCfgStr::convert(const std::string&name, const std::vector<int32_t>& data, int splen, int index)
{
    base_vector(name, data, splen, index);
}

void JsonCfgStr::convert(const std::string&name, const std::vector<uint32_t>& data, int splen, int index)
{
    base_vector(name, data, splen, index);
}

void JsonCfgStr::convert(const std::string&name, const std::vector<int64_t>& data, int splen, int index)
{
    base_vector(name, data, splen, index);
}

void JsonCfgStr::convert(const std::string&name, const std::vector<uint64_t>& data, int splen, int index)
{
    base_vector(name, data, splen, index);
}

void JsonCfgStr::convert(const std::string&name, const std::vector<float>& data, int splen, int index)
{
    base_vector(name, data, splen, index);
}

void JsonCfgStr::convert(const std::string&name, const std::vector<double>& data, int splen, int index)
{
    base_vector(name, data, splen, index);
}

void JsonCfgStr::convert(const std::string&name, const std::vector<bool>& data, int splen, int index)
{
    base_vector(name, data, splen, index);
}

void JsonCfgStr::convert(const std::string&name, const std::vector<std::string>& data, int splen, int index)
{
    linefeed(index);
    _ss<<std::string(splen, ' ')<<_key<<name<<_key<<_assign<<_vecb;
    for (typename std::vector<std::string>::const_iterator it=data.begin(); it!=data.end(); ++it) {
        if (it != data.begin()) {
            _ss<<',';
        }
        _ss<<'"'<<*it<<'"';
    }
    _ss<<_vece;
}


// base type set
void JsonCfgStr::convert(const std::string&name, const std::set<int16_t>& data, int splen, int index)
{
    base_set(name, data, splen, index);
}

void JsonCfgStr::convert(const std::string&name, const std::set<uint16_t>& data, int splen, int index)
{
    base_set(name, data, splen, index);
}

void JsonCfgStr::convert(const std::string&name, const std::set<int32_t>& data, int splen, int index)
{
    base_set(name, data, splen, index);
}

void JsonCfgStr::convert(const std::string&name, const std::set<uint32_t>& data, int splen, int index)
{
    base_set(name, data, splen, index);
}

void JsonCfgStr::convert(const std::string&name, const std::set<int64_t>& data, int splen, int index)
{
    base_set(name, data, splen, index);
}

void JsonCfgStr::convert(const std::string&name, const std::set<uint64_t>& data, int splen, int index)
{
    base_set(name, data, splen, index);
}

void JsonCfgStr::convert(const std::string&name, const std::set<float>& data, int splen, int index)
{
    base_set(name, data, splen, index);
}

void JsonCfgStr::convert(const std::string&name, const std::set<double>& data, int splen, int index)
{
    base_set(name, data, splen, index);
}

void JsonCfgStr::convert(const std::string&name, const std::set<bool>& data, int splen, int index)
{
    base_set(name, data, splen, index);
}

void JsonCfgStr::convert(const std::string&name, const std::set<std::string>& data, int splen, int index)
{
    linefeed(index);
    _ss<<std::string(splen, ' ')<<_key<<name<<_key<<_assign<<_vecb;
    for (typename std::set<std::string>::const_iterator it=data.begin(); it!=data.end(); ++it) {
        if (it != data.begin()) {
            _ss<<',';
        }
        _ss<<'"'<<*it<<'"';
    }
    _ss<<_vece;
}




}



