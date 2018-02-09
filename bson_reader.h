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

#include <map>
#include <vector>
#include <string.h>

#include "xreader.h"

namespace x2struct {

struct BsonDoc;
struct BsonValue;

class BsonReader:public XReader<BsonReader> {
public:
    using xdoc_type::convert;
    BsonReader(const uint8_t*data, size_t length, bool copy=true);
    BsonReader(const std::string&data, bool copy=true);
    ~BsonReader();

    std::string json() const;
public:
    void convert(std::string &val);
    void convert(bool &val);
    void convert(int16_t &val);
    void convert(uint16_t &val);
    void convert(int32_t &val);
    void convert(uint32_t &val);
    void convert(int64_t &val);
    void convert(uint64_t &val);
    void convert(double &val);
    void convert(float &val);

    const std::string& type();
    bool has(const char*key);
    size_t size(bool to_vec=true);
    BsonReader operator[](const char *key);
    BsonReader operator[](size_t index);
    BsonReader begin();
    BsonReader next();
    operator bool() const;

private:
    void init(const uint8_t*data, size_t length, bool copy);
    BsonReader(BsonValue* val, const BsonReader*parent, const char*key);
    BsonReader(BsonValue* val, const BsonReader*parent, size_t index);
    void init_iter(bool top);

    const uint8_t* _data;
    BsonDoc *_doc;
    mutable BsonValue *_val;
};

}


