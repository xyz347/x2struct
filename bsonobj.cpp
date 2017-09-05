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
#include <fstream>
#include <mongo/bson/bson.h>
#include "bsonobj.hpp"


namespace x2struct {

using namespace ::std;
using namespace mongo;

BsonObj::BsonObj(const BSONObj&bobj):XObj("bson"), _v(bobj),_e(*(mongo::BSONElement*)0),_isobj(true)
{
}

BsonObj::BsonObj(const mongo::BSONElement&bele):XObj("bson"),_v(*(BSONObj*)0),_e(bele),_isobj(false)
{
}

BsonObj::~BsonObj()
{
}

void BsonObj::convert(std::string &val)
{
    _e.Val(val);
}

void BsonObj::convert(bool &val)
{
    _e.Val(val);
}

void BsonObj::convert(int32_t &val)
{
    val = (int32_t)_e.numberInt();
}

void BsonObj::convert(uint32_t &val)
{
    val = (uint32_t)_e.numberInt();
}

void BsonObj::convert(int16_t &val)
{
    val = (int16_t)_e.numberInt();
}

void BsonObj::convert(uint16_t &val)
{
    val = (uint16_t)_e.numberInt();
}

void BsonObj::convert(int64_t &val)
{
    val = (int64_t)_e.numberLong();
}

void BsonObj::convert(uint64_t &val)
{
    val = (uint64_t)_e.numberLong();
}

void BsonObj::convert(double &val)
{
    val = _e.numberDouble();
}

void BsonObj::convert(float &val)
{
    val = (float)_e.numberDouble();
}

bool BsonObj::has(const std::string&key)
{
    if (_isobj) {
        return _v.hasField(key);
    } else if (!_e.isABSONObj()) {
        return false;
    } else {
        return _e.embeddedObject().hasField(key);
    }
}

size_t BsonObj::size(bool&isarray)
{
    isarray = false;
    if (_isobj) {
        return 1;
    } else {
        try {
            size_t s = _e.Array().size();
            isarray = true;
            return s;
        } catch (...) {
            return 1;
        }
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
        _ec.push_back(_e.Array()[index]);
        BsonObj obj(_ec[_ec.size()-1]);
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
    bool find = false;
    if (_isobj && _v.hasField(key)) {
        _ec.push_back(_v[key]);
        find = true;
    } else if (!_isobj && _e.isABSONObj() && _e.embeddedObject().hasField(key)) {
        _ec.push_back(_e.embeddedObject()[key]);
        find = true;
    }

    if (find) {
        BsonObj obj(_ec[_ec.size()-1]);
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
    std::set<std::string> fs;

    if (_isobj) {
        _v.getFieldNames(fs);
    } else if (!_isobj && _e.isABSONObj()) {
        _e.embeddedObject().getFieldNames(fs);
    }

    for (auto iter=fs.begin(); iter!=fs.end(); ++iter) {
        fields.push_back(*iter);
    }
}

}
