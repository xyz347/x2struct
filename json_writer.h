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

#include <string>
#include <vector>
#include <set>
#include <map>

#include "xtypes.h"

// typedef 不好 forward declare
#ifndef JSON_WRITER_BUFFER
#define JSON_WRITER_BUFFER void
#define JSON_WRITER_WRITER void
#define JSON_WRITER_PRETTY void
#endif

namespace x2struct {

class JsonWriter {
public:
    JsonWriter(int identCount=0, char identChar=' ');
    ~JsonWriter();
public:
    std::string toStr();

    void set_key(const char*key);
    void array_begin();
    void array_end();
    void object_begin();
    void object_end();
    const std::string&type();

    JsonWriter& convert(const char*key, const std::string &val);
    JsonWriter& convert(const char*key, bool val);
    JsonWriter& convert(const char*key, int16_t val);
    JsonWriter& convert(const char*key, uint16_t val);
    JsonWriter& convert(const char*key, int32_t val);
    JsonWriter& convert(const char*key, uint32_t val);
    JsonWriter& convert(const char*key, int64_t val);
    JsonWriter& convert(const char*key, uint64_t val);
    JsonWriter& convert(const char*key, double val);
    JsonWriter& convert(const char*key, float val);

    template<typename T>
    JsonWriter& convert(const char*key, const std::vector<T>&data) {
        set_key(key);
        this->array_begin();
        for (size_t i=0; i<data.size(); ++i) {
            this->convert("", data[i]);
        }
        this->array_end();
        return *this;
    }

    template<typename T>
    JsonWriter& convert(const char*key, const std::set<T>&data) {
        set_key(key);
        this->array_begin();
        for (typename std::set<T>::const_iterator it=data.begin(); it!=data.end(); ++it) {
            this->convert("", *it);
        }
        this->array_end();
        return *this;
    }

    template <typename T>
    void convert(const char*key, const std::map<std::string, T> &data) {
        set_key(key);
        this->object_begin();
        for (typename std::map<std::string,T>::const_iterator iter=data.begin(); iter!=data.end(); ++iter) {
            this->convert(iter->first, iter->second);
        }
        this->object_end();
    }

    template <typename KEY, typename T>
    void convert(const char*key, const std::map<KEY, T> &data) {
        set_key(key);
        this->object_begin();
        for (typename std::map<KEY,T>::const_iterator iter=data.begin(); iter!=data.end(); ++iter) {
            std::string _k =tostr(iter->first);
            this->convert(_k.c_str(), iter->second);
        }
        this->object_end();
    }

    template <typename T>
    void convert(const char*key, const T& data) {
        set_key(key);
        this->object_begin();
        data.__struct_to_str(*this, key);
        this->object_end();
    }

    template <typename T>
    void convert(const char*key, const XType<T>& data) {
        data.__struct_to_str(*this, key);
    }

private:
    JSON_WRITER_BUFFER* _buf;
    JSON_WRITER_WRITER* _writer;
    JSON_WRITER_PRETTY* _pretty;
};

}


