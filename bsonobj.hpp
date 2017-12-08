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

#pragma once

#include <stdint.h>
#include "xobj.hpp"


namespace x2struct {

struct BsonIter;

class BsonObj:public XObj {
public:
    BsonObj(const uint8_t*data, size_t length);
    BsonObj(void*iter); // type bson_iter_t, but bson_iter_t is unnamed struct, could not forward declare
    BsonObj(const BsonObj&obj);
    ~BsonObj();
public:
    virtual bool support_next() const {return true;}
    virtual XObj* next();
public: // convert
    virtual void convert(std::string &val);
    virtual void convert(bool &val);
    virtual void convert(int16_t &val);
    virtual void convert(uint16_t &val);
    virtual void convert(int32_t &val);
    virtual void convert(uint32_t &val);
    virtual void convert(int64_t &val);
    virtual void convert(uint64_t &val);
    virtual void convert(double &val);
    virtual void convert(float &val);

public:
    virtual bool has(const std::string&key);
    virtual size_t size();
    virtual XObj& operator[](const std::string&key);
    virtual XObj& subobj(size_t index);
    virtual XObj& subobj(const std::string&key);
    virtual void getsubfields(std::vector<std::string>& fields);
public:
    virtual std::string attribute(const std::string&key);
private:
    size_t size(bool&isarray);
    void init_iter(bool top);
private:
    //bool  _isobj;
    std::string _data;
    mutable BsonIter *_iter;
    std::vector<BsonObj> _gc;   //垃圾回收
};

}


