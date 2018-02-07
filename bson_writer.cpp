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
#include <libbson-1.0/bson.h>
#include "bson_writer.h"

using namespace std;

namespace x2struct {

BsonWriter::BsonWriter(const char* key, _bson_t*parent, int type)
{
    _parent = parent;
    _bson = new bson_t;
    _type = type;
    if (0 != parent) {
        if (type == doc) {
            bson_append_document_begin(parent, key, strlen(key), _bson);
        } else {
            bson_append_array_begin(parent, key, strlen(key), _bson);
        }
    } else {
        bson_init(_bson);
    }
}

BsonWriter::BsonWriter(const BsonWriter&bs)
{
    *this = bs;
    bs._parent = 0;
    bs._bson = 0;
}


BsonWriter::~BsonWriter()
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

string BsonWriter::toStr()const 
{
    return std::string((const char*)bson_get_data(_bson), _bson->len);
}

string BsonWriter::json()const 
{
    size_t len;
    char *jstr = bson_as_json(_bson, &len);
    std::string ret(jstr);
    bson_free(jstr);

    return ret;
}

BsonWriter& BsonWriter::convert(const char*key, int16_t data)
{
    bson_append_int32(_bson, key, strlen(key), (int32_t)data);
    return *this;
}

BsonWriter& BsonWriter::convert(const char*key, uint16_t data)
{
    bson_append_int32(_bson, key, strlen(key), (int32_t)data);
    return *this;
}

BsonWriter& BsonWriter::convert(const char*key, int32_t data)
{
    bson_append_int32(_bson, key, strlen(key), (int32_t)data);
    return *this;
}

BsonWriter& BsonWriter::convert(const char*key, uint32_t data)
{
    bson_append_int32(_bson, key, strlen(key), (int32_t)data);
    return *this;
}

BsonWriter& BsonWriter::convert(const char*key, int64_t data)
{
    bson_append_int64(_bson, key, strlen(key), (int64_t)data);
    return *this;
}

BsonWriter& BsonWriter::convert(const char*key, uint64_t data)
{
    bson_append_int64(_bson, key, strlen(key), (int64_t)data);
    return *this;
}

BsonWriter& BsonWriter::convert(const char*key, float data)
{
    bson_append_double(_bson, key, strlen(key), (double)data);
    return *this;
}

BsonWriter& BsonWriter::convert(const char*key, double data)
{
    bson_append_double(_bson, key, strlen(key), (double)data);
    return *this;
}

BsonWriter& BsonWriter::convert(const char*key, bool data)
{
    bson_append_bool(_bson, key, strlen(key), data);
    return *this;
}

BsonWriter& BsonWriter::convert(const char*key, const string& data)
{
    bson_append_utf8(_bson, key, strlen(key), (const char*)data.data(), data.length());
    return *this;
}

BsonWriter& BsonWriter::convert(const char*key, const char* data)
{
    std::string d(data);
    return convert(key, d);
}

BsonWriter& BsonWriter::convert(const char*key, const BsonWriter& data)
{
    bson_append_document(_bson, key, strlen(key), data._bson);
    return *this;
}

const std::string& BsonWriter::type()
{
    static string t("bson");
    return t;
}


}



