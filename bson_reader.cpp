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

#include <stdexcept>
#include <map>
#include <vector>
#include <list>

#include "thirdparty/libbson/include/libbson-1.0/bson.h"

#include "util.h"
#include "bson_reader.h"
//#include <iostream>

using namespace ::std;

namespace x2struct {

typedef map<const char*, bson_iter_t, cmp_str> objs_type;

struct BsonDoc;
struct BsonValue {
    bson_iter_t root;
    objs_type objs;
    vector<bson_iter_t> vecs;
    objs_type::iterator iter;
    BsonDoc* doc;
};

struct BsonDoc {
    std::string data;
    std::list<BsonValue> vals;
    BsonValue *Get(const bson_iter_t*iter, bool top=false) {
        BsonValue _tmp_val;
        vals.push_back(_tmp_val);
        BsonValue *val = &vals.back();
        memcpy(&val->root, iter, sizeof(val->root));

        bson_iter_t sub;
        if (top) {
            memcpy(&sub, &val->root, sizeof(sub));
            while (bson_iter_next(&sub)) {
                val->objs[bson_iter_key(&sub)] = sub;
            }
        } else if (bson_iter_recurse(&val->root, &sub)) {
            if (BSON_TYPE_ARRAY != bson_iter_type(&val->root)) {
                while (bson_iter_next(&sub)) {
                    val->objs[bson_iter_key(&sub)] = sub;
                }
            } else {
                while (bson_iter_next(&sub)) {
                    val->vecs.push_back(sub);
                }
            }
        }
        val->doc = this;
        return val;
    }
};

void BsonReader::init(const uint8_t*data, size_t length, bool copy)
{
    bson_t b; // local is ok
    length = (length>0)?length:BSON_UINT32_TO_LE(*(int32_t*)data);
    if (copy) {
        string tmp(string((const char*)data, length));
        _doc->data.swap(tmp);
        bson_init_static(&b, (const uint8_t*)_doc->data.data(), _doc->data.length());
        _data = (const uint8_t*)_doc->data.data();
    } else {
        bson_init_static(&b, data, length);
        _data = data;
    }

    bson_iter_t root;
    bson_iter_init(&root, &b);
    _val = _doc->Get(&root, true);
}

BsonReader::BsonReader(const uint8_t*data, size_t length, bool copy)
    :xdoc_type(0, "")
    ,_doc(new BsonDoc)
{
    init(data, length, copy);
}

BsonReader::BsonReader(const std::string&data, bool copy)
    :xdoc_type(0, "")
    ,_doc(new BsonDoc)
{
    init((const uint8_t*)data.data(), data.length(), copy);
}

BsonReader::BsonReader(BsonValue* val, const BsonReader*parent, const char*key):xdoc_type(parent, key),_val(val)
{
    _doc = 0;
    _data = 0;
}
BsonReader::BsonReader(BsonValue* val, const BsonReader*parent, size_t index):xdoc_type(parent, index),_val(val)
{
    _doc = 0;
    _data = 0;
}

BsonReader::~BsonReader()
{
    if (0 != _doc) {
        delete _doc;
    }
}

void BsonReader::convert(std::string &val)
{
    uint32_t length;
    const char* data = bson_iter_utf8(&_val->root, &length);
    if (0 != data) {
        val = string(data, length);
    }
}

void BsonReader::convert(bool &val)
{
    val = (bool)bson_iter_as_int64(&_val->root);
}

void BsonReader::convert(int32_t &val)
{
    val = (int32_t)bson_iter_as_int64(&_val->root);
}

void BsonReader::convert(uint32_t &val)
{
    val = (uint32_t)bson_iter_as_int64(&_val->root);
}

void BsonReader::convert(int16_t &val)
{
    val = (int16_t)bson_iter_as_int64(&_val->root);
}

void BsonReader::convert(uint16_t &val)
{
    val = (uint16_t)bson_iter_as_int64(&_val->root);
}

void BsonReader::convert(int64_t &val)
{
    val = bson_iter_as_int64(&_val->root);
}

void BsonReader::convert(uint64_t &val)
{
    val = (uint64_t)bson_iter_as_int64(&_val->root);
}

void BsonReader::convert(double &val)
{
    val = bson_iter_double(&_val->root);
}

void BsonReader::convert(float &val)
{
    val = (float)bson_iter_double(&_val->root);
}

bool BsonReader::has(const char* key)
{
    bool ret = _val->objs.find(key)!=_val->objs.end();
    return ret;
}

size_t BsonReader::size(bool to_vec)
{
    if (BSON_TYPE_ARRAY==bson_iter_type(&_val->root)) {
        return _val->vecs.size();
    } else {
        return 0;
    }
}

BsonReader BsonReader::operator[](const char* key)
{
    objs_type::iterator iter = _val->objs.find(key);
    if (iter != _val->objs.end()) {
        return BsonReader(_val->doc->Get(&iter->second), this, key);
    } else {
        throw std::runtime_error(string("Did not have ")+key);
    }
    return BsonReader(0, 0, "");
}

BsonReader BsonReader::operator[](size_t index)
{
    if (index<_val->vecs.size()) {
        return BsonReader(_val->doc->Get(&_val->vecs[index]), this, index);
    } else {
        throw std::runtime_error("Out of index");
    }

    return BsonReader(0, 0, "");
}

BsonReader::operator bool() const
{
    return 0!=_val;
}

BsonReader BsonReader::begin()
{
    _val->iter = _val->objs.begin();
    if (_val->iter != _val->objs.end()) {
        return BsonReader(_val->doc->Get(&_val->iter->second), this, _val->iter->first);
    } else {
        return BsonReader(0, this, "");
    }
}

BsonReader BsonReader::next()
{
    if (0==_parent || 0==_parent->_val) {
        throw std::runtime_error("parent null");
    } else {
        ++(_parent->_val->iter);
    }
    if (_parent->_val->iter != _parent->_val->objs.end()) {
        return BsonReader(_val->doc->Get(&_parent->_val->iter->second), _parent, _parent->_val->iter->first);
    } else {
        return BsonReader(0, _parent, "");
    }
}

const std::string& BsonReader::type()
{
    static std::string t("bson");
    return t;
}

std::string BsonReader::json() const
{
    if (0 == _data) {
        return "";
    }
    size_t length=BSON_UINT32_TO_LE(*(int32_t*)_data);
    bson_t b; // local is ok
    bson_init_static(&b, _data, length);

    size_t s;
    char *jstr = bson_as_json(&b, &s);
    if (jstr) {
        std::string j(jstr);
        bson_free(jstr);
        return j;
    } else {
        return "";
    }
}

}

