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

#include "util.hpp"
#include "xstr.hpp"

struct _bson_t;

namespace x2struct {

class BsonStr:public XStr {
    enum {
        top,
        doc,
        array
    };
public:
    BsonStr(const std::string&name="", _bson_t*parent=0, int type=top);
    BsonStr(const BsonStr&bs);
    ~BsonStr();

public:
    void begin(const std::string&root, int splen);
    void end(const std::string&root, int splen);
    int  space();
    std::string toStr() const;
    std::string json() const;
public:
    BsonStr& convert(const std::string&name, const BsonStr& data, int space=0, int index=0);

    BsonStr& convert(const std::string&name, const char* data, int space=0, int index=0);
    BsonStr& convert(const std::string&name, const std::string& data, int space=0, int index=0);

    // base type
    BsonStr& convert(const std::string&name, int16_t data, int splen=0, int index=0);
    BsonStr& convert(const std::string&name, uint16_t data, int splen=0, int index=0);
    BsonStr& convert(const std::string&name, int32_t data, int splen=0, int index=0);
    BsonStr& convert(const std::string&name, uint32_t data, int splen=0, int index=0);
    BsonStr& convert(const std::string&name, int64_t data, int splen=0, int index=0);
    BsonStr& convert(const std::string&name, uint64_t data, int splen=0, int index=0);
    BsonStr& convert(const std::string&name, float data, int splen=0, int index=0);
    BsonStr& convert(const std::string&name, double data, int splen=0, int index=0);
    BsonStr& convert(const std::string&name, bool data, int splen=0, int index=0);

    template<typename T>
    BsonStr& convert(const std::string&name, const std::vector<T>&data, int splen=0, int index=0) {
        BsonStr child(name, _bson, array);
        for (size_t i=0; i<data.size(); ++i) {
            child.convert(tostr(i), data[i], 0,0);
        }
        return *this;
    }
    template<typename T>
    BsonStr& convert(const std::string&name, const std::set<T>&data, int splen=0, int index=0) {
        BsonStr child(name, _bson, array);
        size_t i = 0;
        for (typename std::set<T>::const_iterator iter=data.begin(); iter!=data.end(); ++iter,++i) {
            child.convert(tostr(i), *iter, 0,0);
        }
        return *this;
    }
    template<typename T>
    BsonStr& convert(const std::string&name, const std::map<std::string, T>&data, int splen=0, int index=0) {
        if (_type!=top || name.length()!=0) {
            BsonStr child(name, _bson, doc);
            for (typename std::map<std::string, T>::const_iterator iter=data.begin(); iter!=data.end(); ++iter) {
                child.convert(iter->first, iter->second, 0,0);
            }
        } else {
            for (typename std::map<std::string, T>::const_iterator iter=data.begin(); iter!=data.end(); ++iter) {
                this->convert(iter->first, iter->second, 0,0);
            }
        }
        return *this;
    }
    template <typename K, typename T>
    BsonStr& convert(const std::string&name, const std::map<K, T> &data, int splen=0, int index=0) {
        if (_type!=top || name.length()!=0) {
            BsonStr child(name, _bson, doc);
            for (typename std::map<K, T>::const_iterator iter=data.begin(); iter!=data.end(); ++iter) {
                child.convert(tostr(iter->first), iter->second, 0,0);
            }
        } else {
            for (typename std::map<K, T>::const_iterator iter=data.begin(); iter!=data.end(); ++iter) {
                this->convert(tostr(iter->first), iter->second, 0,0);
            }
        }
        return *this;
    }

    template <typename T>
    BsonStr& convert(const std::string&name, const T& data, int splen=0, int index=0) {
        if (_type!=top || name.length()!=0) {
            BsonStr child(name, _bson, doc);
            data.__struct_to_str(child, "", 0);
        } else {
            data.__struct_to_str(*this, "", 0);
        }
        return *this;
    }
private:
    mutable _bson_t* _parent;
    mutable _bson_t* _bson;
    int _type;
};

}

