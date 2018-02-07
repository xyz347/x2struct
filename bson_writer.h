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
#include <string>
#include <map>
#include <vector>
#include <set>

#include "util.h"
#include "xtypes.h"

struct _bson_t;

namespace x2struct {

class BsonWriter {
    enum {
        top,
        doc,
        array
    };
public:
    BsonWriter(const char*name="", _bson_t*parent=0, int type=top);
    BsonWriter(const BsonWriter&bs);
    ~BsonWriter();

public:
    std::string toStr() const;
    std::string json() const;

    const std::string&type();

    BsonWriter& convert(const char*key, const BsonWriter& data);
    BsonWriter& convert(const char*key, const char* data);
    BsonWriter& convert(const char*key, const std::string& data);
    BsonWriter& convert(const char*key, int16_t data);
    BsonWriter& convert(const char*key, uint16_t data);
    BsonWriter& convert(const char*key, int32_t data);
    BsonWriter& convert(const char*key, uint32_t data);
    BsonWriter& convert(const char*key, int64_t data);
    BsonWriter& convert(const char*key, uint64_t data);
    BsonWriter& convert(const char*key, float data);
    BsonWriter& convert(const char*key, double data);
    BsonWriter& convert(const char*key, bool data);

    template<typename T>
    BsonWriter& convert(const char*key, const std::vector<T>&data) {
        BsonWriter child(key, _bson, array);
        for (size_t i=0; i<data.size(); ++i) {
            child.convert(tostr(i).c_str(), data[i]);
        }
        return *this;
    }
    template<typename T>
    BsonWriter& convert(const char*key, const std::set<T>&data) {
        BsonWriter child(key, _bson, array);
        size_t i = 0;
        for (typename std::set<T>::const_iterator iter=data.begin(); iter!=data.end(); ++iter,++i) {
            child.convert(tostr(i).c_str(), *iter);
        }
        return *this;
    }
    template<typename T>
    BsonWriter& convert(const char*key, const std::map<std::string, T>&data) {
        if (_type!=top || key[0]!='\0') {
            BsonWriter child(key, _bson, doc);
            for (typename std::map<std::string, T>::const_iterator iter=data.begin(); iter!=data.end(); ++iter) {
                child.convert(iter->first.c_str(), iter->second);
            }
        } else {
            for (typename std::map<std::string, T>::const_iterator iter=data.begin(); iter!=data.end(); ++iter) {
                this->convert(iter->first.c_str(), iter->second);
            }
        }
        return *this;
    }
    template <typename K, typename T>
    BsonWriter& convert(const char*key, const std::map<K, T> &data) {
        if (_type!=top || key[0]!='\0') {
            BsonWriter child(key, _bson, doc);
            for (typename std::map<K, T>::const_iterator iter=data.begin(); iter!=data.end(); ++iter) {
                child.convert(tostr(iter->first).c_str(), iter->second);
            }
        } else {
            for (typename std::map<K, T>::const_iterator iter=data.begin(); iter!=data.end(); ++iter) {
                this->convert(tostr(iter->first).c_str(), iter->second);
            }
        }
        return *this;
    }

    template <typename T>
    BsonWriter& convert(const char*key, const T& data) {
        if (_type!=top || key[0]!='\0') {
            BsonWriter child(key, _bson, doc);
            data.__struct_to_str(child, "");
        } else {
            data.__struct_to_str(*this, "");
        }
        return *this;
    }

    template <typename T>
    void convert(const char*key, const XType<T>& data) {
        data.__struct_to_str(*this, key);
    }
private:
    mutable _bson_t* _parent;
    mutable _bson_t* _bson;
    int _type;
};

}

