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
#include <libbson-1.0/bson.h>
#include "bsonobj.hpp"
//#include <iostream>


using namespace ::std;

namespace x2struct {

struct BsonIter {
    bson_iter_t root;
    map<string, bson_iter_t> objs;
    vector<bson_iter_t> vecs;
};

BsonObj::BsonObj(const uint8_t*data, size_t length)
    :XObj("bson")
    ,_data((const char*)data, (length>0)?length:BSON_UINT32_TO_LE(*(int32_t*)data))
    ,_iter(0)
{
    bson_t b; // local is ok
    bson_init_static(&b, (const uint8_t*)_data.data(), _data.length());
    _iter = new BsonIter;
    bson_iter_init(&_iter->root, &b);
    init_iter(true);
}

BsonObj::BsonObj(void*iter):XObj("bson")
{
    _iter = new BsonIter;
    memcpy(&_iter->root, iter, sizeof(_iter->root));
    init_iter(false);
}

BsonObj::BsonObj(const BsonObj&obj):XObj("bson")
{
    *this = obj;
    obj._iter = 0;
}

void BsonObj::init_iter(bool top)
{
    bson_iter_t sub;
    if (top) {
        memcpy(&sub, &_iter->root, sizeof(sub));
        while (bson_iter_next(&sub)) {
            _iter->objs[bson_iter_key(&sub)] = sub;
        }
    } else if (bson_iter_recurse(&_iter->root, &sub)) {
        if (BSON_TYPE_ARRAY != bson_iter_type(&_iter->root)) {
            while (bson_iter_next(&sub)) {
                _iter->objs[bson_iter_key(&sub)] = sub;
            }
        } else {
            while (bson_iter_next(&sub)) {
                _iter->vecs.push_back(sub);
            }
        }
    }
}

BsonObj::~BsonObj()
{
    if (0 != _iter) {
        delete _iter;
    }
}

void BsonObj::convert(std::string &val)
{
    uint32_t length;
    const char* data = bson_iter_utf8(&_iter->root, &length);
    if (0 != data) {
        val = string(data, length);
    }
}

void BsonObj::convert(bool &val)
{
    val = bson_iter_bool(&_iter->root);
}

void BsonObj::convert(int32_t &val)
{
    val = bson_iter_int32(&_iter->root);
}

void BsonObj::convert(uint32_t &val)
{
    val = (uint32_t)bson_iter_int32(&_iter->root);
}

void BsonObj::convert(int16_t &val)
{
    val = (int16_t)bson_iter_int32(&_iter->root);
}

void BsonObj::convert(uint16_t &val)
{
    val = (uint16_t)bson_iter_int32(&_iter->root);
}

void BsonObj::convert(int64_t &val)
{
    val = bson_iter_as_int64(&_iter->root);
}

void BsonObj::convert(uint64_t &val)
{
    val = (uint64_t)bson_iter_as_int64(&_iter->root);
}

void BsonObj::convert(double &val)
{
    val = bson_iter_double(&_iter->root);
}

void BsonObj::convert(float &val)
{
    val = (float)bson_iter_double(&_iter->root);
}

bool BsonObj::has(const std::string&key)
{
    return _iter->objs.find(key)!=_iter->objs.end();
}

size_t BsonObj::size(bool&isarray)
{
    isarray = (BSON_TYPE_ARRAY==bson_iter_type(&_iter->root));
    if (isarray) {
        return _iter->vecs.size();
    } else {
        return 1;
    }
}

size_t BsonObj::size()
{
    bool isarray;
    return size(isarray);
}


XObj& BsonObj::operator[](const std::string&key)
{
    return subobj(key);
}

XObj& BsonObj::subobj(size_t index)
{
    bool isarray;
    size_t asize = size(isarray);
    if (isarray && (index<asize)) {
        BsonObj obj(&_iter->vecs[index]);
        obj._key = _key;
        obj.set_path(_path, index);
        _gc.push_back(obj);
        return _gc[_gc.size()-1];
    } else if (!isarray && index==0) {
        return *this;
    }

    throw std::runtime_error("Out of index");
    return *((XObj*)0);
}

XObj& BsonObj::subobj(const std::string&key)
{
    map<string,bson_iter_t>::iterator iter = _iter->objs.find(key);
    if (iter != _iter->objs.end()) {
        BsonObj obj(&iter->second);
        obj._key = key;
        obj.set_path(_path, key);
        _gc.push_back(obj);
        return _gc[_gc.size()-1];
    } else {
        throw std::runtime_error("Did not have "+key);
        return *((XObj*)0);
    }
}

string BsonObj::attribute(const std::string&key)
{
    try {
        string val;
        subobj(key).convert(val);
        return val;
    } catch (...) {
        return "";
    }
}

void BsonObj::getsubfields(std::vector<std::string>& fields)
{
    map<string,bson_iter_t>::iterator iter = _iter->objs.begin();
    for (;iter!=_iter->objs.end(); ++iter) {
        const char *name = bson_iter_key(&iter->second);
        fields.push_back(name);
    }
}

std::string BsonObj::json() const
{
    bson_t b; // local is ok
    bson_init_static(&b, (const uint8_t*)_data.data(), _data.length());

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

