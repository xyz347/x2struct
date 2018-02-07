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

#include "xreader.h"

// typedef 不好 forward declare
#ifndef JSON_READER_DOCUMENT
#define JSON_READER_DOCUMENT void
#define JSON_READER_VALUE void
#define JSON_READER_ITERATOR void
#endif

namespace x2struct {


class JsonReader:public XReader<JsonReader> {
public:
    using xdoc_type::convert;
    JsonReader(const std::string& str, bool isfile=false);
    ~JsonReader();
public: // convert
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
    JsonReader operator[](const char *key);
    JsonReader operator[](size_t index);
    JsonReader begin();
    JsonReader next();
    operator bool() const;

private:
    JsonReader(const JSON_READER_VALUE* val, const JsonReader*parent, const char*key);
    JsonReader(const JSON_READER_VALUE* val, const JsonReader*parent, size_t index);
    void init();

    JSON_READER_DOCUMENT* _doc;
    const JSON_READER_VALUE* _val;
    mutable JSON_READER_ITERATOR* _iter;
};

}


