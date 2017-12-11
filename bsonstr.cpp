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


#include <libbson-1.0/bson.h>

#include "bsonstr.hpp"

using namespace std;

namespace x2struct {

BsonStr::BsonStr(const std::string&name, _bson_t*parent, int type):XStr("bson")
{
    _parent = parent;
    _bson = new bson_t;
    _type = type;
    if (0 != parent) {
        if (type == doc) {
            bson_append_document_begin(parent, name.c_str(), name.length(), _bson);
        } else {
            bson_append_array_begin(parent, name.c_str(), name.length(), _bson);
        }
    } else {
        bson_init(_bson);
    }
}

BsonStr::BsonStr(const BsonStr&bs):XStr("bson")
{
    *this = bs;
    bs._parent = 0;
    bs._bson = 0;
}


BsonStr::~BsonStr()
{
    if (0 != _bson) {
        if (_type == doc) {
            bson_append_document_end(_parent, _bson);
        } else if (_type == array) {
            bson_append_array_end(_parent, _bson);
        } else {
            bson_destroy(_bson);
        }
        delete _bson;
    }
}

void BsonStr::begin(const string&root, int splen)
{
}

void BsonStr::end(const string&root, int splen)
{
}

int BsonStr::space()
{
    return 0;
}

string BsonStr::toStr()const 
{
    return std::string((const char*)bson_get_data(_bson), _bson->len);
}

BsonStr& BsonStr::convert(const string&name, int16_t data, int splen, int index)
{
    bson_append_int32(_bson, name.c_str(), name.length(), (int32_t)data);
    return *this;
}

BsonStr& BsonStr::convert(const string&name, uint16_t data, int splen, int index)
{
    bson_append_int32(_bson, name.c_str(), name.length(), (int32_t)data);
    return *this;
}

BsonStr& BsonStr::convert(const string&name, int32_t data, int splen, int index)
{
    bson_append_int32(_bson, name.c_str(), name.length(), (int32_t)data);
    return *this;
}

BsonStr& BsonStr::convert(const string&name, uint32_t data, int splen, int index)
{
    bson_append_int32(_bson, name.c_str(), name.length(), (int32_t)data);
    return *this;
}

BsonStr& BsonStr::convert(const std::string&name, int64_t data, int splen, int index)
{
    bson_append_int64(_bson, name.c_str(), name.length(), (int64_t)data);
    return *this;
}

BsonStr& BsonStr::convert(const std::string&name, uint64_t data, int splen, int index)
{
    bson_append_int64(_bson, name.c_str(), name.length(), (int64_t)data);
    return *this;
}

BsonStr& BsonStr::convert(const std::string&name, float data, int splen, int index)
{
    bson_append_double(_bson, name.c_str(), name.length(), (double)data);
    return *this;
}

BsonStr& BsonStr::convert(const std::string&name, double data, int splen, int index)
{
    bson_append_double(_bson, name.c_str(), name.length(), (double)data);
    return *this;
}

BsonStr& BsonStr::convert(const std::string&name, bool data, int splen, int index)
{
    bson_append_bool(_bson, name.c_str(), name.length(), data);
    return *this;
}

BsonStr& BsonStr::convert(const string&name, const string& data, int splen, int index)
{
    bson_append_utf8(_bson, name.c_str(), name.length(), (const char*)data.data(), data.length());
    return *this;
}

BsonStr& BsonStr::convert(const string&name, const char* data, int splen, int index)
{
    std::string d(data);
    return convert(name, d, splen, index);
}

BsonStr& BsonStr::convert(const std::string&name, const BsonStr& data, int space, int index)
{
    bson_append_document(_bson, name.c_str(), name.length(), data._bson);
    return *this;
}


}



