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

#define JSON_READER_DOCUMENT rapidjson::Document
#define JSON_READER_VALUE  rapidjson::Value
#define JSON_READER_ITERATOR rapidjson::Value::ConstMemberIterator

#include <stdexcept>
#include <fstream>
#include <rapidjson/document.h>

#include "json_reader.h"

namespace x2struct {

using namespace ::std;

JsonReader::JsonReader(const std::string& str, bool isfile):xdoc_type(0, ""),_doc(new JSON_READER_DOCUMENT),_val(_doc)
{
    string err;
    string data;

    do {
        if (isfile) {
            std::ifstream fs(str.c_str(), std::ifstream::binary);
            if (!fs) {
                err = "Open file["+str+"] fail.";
                break;
            }
            std::string _tmp((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
            data.swap(_tmp);
            _doc->Parse(data);
        } else  {
            _doc->Parse(str);
        }

        if (_doc->HasParseError()) {
            size_t offset = _doc->GetErrorOffset();
            if  (isfile) {
                string err_info = data.substr(offset, offset+32);
                err = "Parse json file ["+str+"] fail. "+err_info;
                break;
            } else {
                string err_info = str.substr(offset, offset+32);
                err = "Parse json string ["+str+"] fail. "+err_info;
                break;
            }
        }
        init();
        return;
    } while (false);

    delete _doc;
    _doc = 0;
    throw std::runtime_error(err);
}

JsonReader::JsonReader(const JSON_READER_VALUE* val, const JsonReader*parent, const char*key):xdoc_type(parent, key),_doc(0),_val(val)
{
    init();
}

JsonReader::JsonReader(const JSON_READER_VALUE* val, const JsonReader*parent, size_t index):xdoc_type(parent, index),_doc(0),_val(val)
{
    init();
}

JsonReader::~JsonReader()
{
    if (0 != _doc) {
        delete _doc;
        _doc = 0;
    }
    if (0 != _iter) {
        delete _iter;
        _iter = 0;
    }
}

void JsonReader::init()
{
    _iter = 0;
}

void JsonReader::convert(std::string &val)
{
    val = _val->GetString();
}

void JsonReader::convert(bool &val)
{
    val = _val->GetBool();
}

void JsonReader::convert(int32_t &val)
{
    val = _val->GetInt();
}

void JsonReader::convert(uint32_t &val)
{
    val = _val->GetUint();
}

void JsonReader::convert(int16_t &val)
{
    val = (int16_t)_val->GetInt();
}

void JsonReader::convert(uint16_t &val)
{
    val = (uint16_t)_val->GetUint();
}

void JsonReader::convert(int64_t &val)
{
    val = _val->GetInt64();
}

void JsonReader::convert(uint64_t &val)
{
    val = _val->GetUint64();
}

void JsonReader::convert(double &val)
{
    val = _val->GetDouble();
}

void JsonReader::convert(float &val)
{
    val = _val->GetFloat();
}

bool JsonReader::has(const char *key)
{
    return _val->HasMember(key);
}

size_t JsonReader::size(bool to_vec)
{
    if (_val->IsArray()) {
        return (size_t)_val->Size();
    } else {
        return 0;
    }
}

JsonReader JsonReader::operator[](const char *key)
{
    if (_val->HasMember(key)) {
        return JsonReader(&(*_val)[key], this, key);
    } else {
        throw std::runtime_error(string("Did not have ")+key);
    }
    return JsonReader(0, 0, "");
}

JsonReader JsonReader::operator[](size_t index)
{
    if (_val->IsArray()) {
        return JsonReader(&(*_val)[(rapidjson::SizeType)index], this, index);
    } else {
        throw std::runtime_error("Out of index");
    }
    return JsonReader(0, 0, "");
}

JsonReader::operator bool() const
{
    return 0!=_val;
}

JsonReader JsonReader::begin()
{
    if (_iter != 0) {
        delete _iter;
    }
    _iter = new(JSON_READER_ITERATOR);
    *_iter = _val->MemberBegin();
    if (*_iter != _val->MemberEnd()) {
        return JsonReader(&(*_iter)->value, this, (*_iter)->name.GetString());
    } else {
        return JsonReader(0, this, "");
    }
}

JsonReader JsonReader::next()
{
    if (0 == _parent) {
        throw std::runtime_error("parent null");
    } else if (0 == _parent->_iter) {
        throw std::runtime_error("parent no iter");
    } else {
        ++(*_parent->_iter);
    }
    if (*_parent->_iter != _parent->_val->MemberEnd()) {
        return JsonReader(&(*_parent->_iter)->value, _parent, (*_parent->_iter)->name.GetString());
    } else {
        return JsonReader(0, _parent, "");
    }
}

const std::string& JsonReader::type()
{
    static std::string t("json");
    return t;
}

}
