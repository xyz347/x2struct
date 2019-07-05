/*
* Copyright (C) 2019 YY Inc. All rights reserved.
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


#ifndef __X_BSON_WRITER_H
#define __X_BSON_WRITER_H


#include "xstdint.h"
#include <string>
#include <map>
#include <vector>
#include <list>
#include <set>

#include "thirdparty/libbson/include/libbson-1.0/bson.h"

#include "util.h"
#include "xtypes.h"
#include "traits.h"

struct _bson_t;

namespace x2struct {

class BsonWriter {
    enum {
        top,
        doc,
        array
    };
public:
    BsonWriter(const char*key="", _bson_t*parent=0, int type=top) {
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
    BsonWriter(const BsonWriter&bs) {
        *this = bs;
        bs._parent = 0;
        bs._bson = 0;
    }
    ~BsonWriter() {
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

public:
    std::string toStr() const {
        return std::string((const char*)bson_get_data(_bson), _bson->len);
    }
    std::string json() const {
        size_t len;
        char *jstr = bson_as_json(_bson, &len);
        std::string ret(jstr);
        bson_free(jstr);

        return ret;
    }

    const std::string&type() {
        static std::string t("bson");
        return t;
    }

    BsonWriter& convert(const char*key, const BsonWriter& data){
        bson_append_document(_bson, key, strlen(key), data._bson);
        return *this;
    }
    BsonWriter& convert(const char*key, const char* data) {
        std::string d(data);
        return convert(key, d);
    }
    BsonWriter& convert(const char*key, const std::string& data) {
        bson_append_utf8(_bson, key, strlen(key), (const char*)data.data(), data.length());
        return *this;
    }
    BsonWriter& convert(const char*key, int16_t data) {
        bson_append_int32(_bson, key, strlen(key), (int32_t)data);
        return *this;
    }
    BsonWriter& convert(const char*key, uint16_t data) {
        bson_append_int32(_bson, key, strlen(key), (int32_t)data);
        return *this;
    }
    BsonWriter& convert(const char*key, int32_t data) {
        bson_append_int32(_bson, key, strlen(key), (int32_t)data);
        return *this;
    }
    BsonWriter& convert(const char*key, uint32_t data) {
        bson_append_int32(_bson, key, strlen(key), (int32_t)data);
        return *this;
    }
    BsonWriter& convert(const char*key, int64_t data) {
        bson_append_int64(_bson, key, strlen(key), (int64_t)data);
        return *this;
    }
    BsonWriter& convert(const char*key, uint64_t data) {
        bson_append_int64(_bson, key, strlen(key), (int64_t)data);
        return *this;
    }
    BsonWriter& convert(const char*key, float data) {
        bson_append_double(_bson, key, strlen(key), (double)data);
        return *this;
    }
    BsonWriter& convert(const char*key, double data) {
        bson_append_double(_bson, key, strlen(key), (double)data);
        return *this;
    }
    BsonWriter& convert(const char*key, bool data) {
        bson_append_bool(_bson, key, strlen(key), data);
        return *this;
    }

    template<typename T>
    BsonWriter& convert(const char*key, const std::vector<T>&data) {
        BsonWriter child(key, _bson, array);
        for (size_t i=0; i<data.size(); ++i) {
            child.convert(Util::tostr(i).c_str(), data[i]);
        }
        return *this;
    }
    template<typename T>
    BsonWriter& convert(const char*key, const std::list<T>&data) {
        BsonWriter child(key, _bson, array);
        size_t i = 0;
        for (typename std::list<T>::const_iterator iter=data.begin(); iter!=data.end(); ++iter,++i) {
            child.convert(Util::tostr(i).c_str(), *iter);
        }
        return *this;
    }
    template<typename T>
    BsonWriter& convert(const char*key, const std::set<T>&data) {
        BsonWriter child(key, _bson, array);
        size_t i = 0;
        for (typename std::set<T>::const_iterator iter=data.begin(); iter!=data.end(); ++iter,++i) {
            child.convert(Util::tostr(i).c_str(), *iter);
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
                child.convert(Util::tostr(iter->first).c_str(), iter->second);
            }
        } else {
            for (typename std::map<K, T>::const_iterator iter=data.begin(); iter!=data.end(); ++iter) {
                this->convert(Util::tostr(iter->first).c_str(), iter->second);
            }
        }
        return *this;
    }


    #ifdef X_SUPPORT_C0X
    // unordered_map
    template<typename T>
    BsonWriter& convert(const char*key, const std::unordered_map<std::string, T>&data) {
        if (_type!=top || key[0]!='\0') {
            BsonWriter child(key, _bson, doc);
            for (typename std::unordered_map<std::string, T>::const_iterator iter=data.begin(); iter!=data.end(); ++iter) {
                child.convert(iter->first.c_str(), iter->second);
            }
        } else {
            for (typename std::unordered_map<std::string, T>::const_iterator iter=data.begin(); iter!=data.end(); ++iter) {
                this->convert(iter->first.c_str(), iter->second);
            }
        }
        return *this;
    }
    // shared_ptr
    template <typename TYPE>
    BsonWriter& convert(const char*key, const std::shared_ptr<TYPE>& val) {
        if (NULL == val.get()) {
            return *this;
        }
        return this->convert(key, *val);
    }

    // class/struct that not defined macro XTOSTRUCT
    template <typename T, typename std::enable_if<!x_has_x2struct<T>::value, int>::type = 0>
    BsonWriter& convert(const char*key, const T& data, x_for_class(T, int) *unused=0) {
        (void)unused;
        if (_type!=top || key[0]!='\0') {
            BsonWriter child(key, _bson, doc);
            x_struct_to_str(child, "", data);
        } else {
            x_struct_to_str(*this, "", data);
        }
        return *this;
    }

    // class/struct that defined macro XTOSTRUCT
    template <typename T, typename std::enable_if<x_has_x2struct<T>::value, int>::type = 0>
    BsonWriter& convert(const char*key, const T& data) {
    #else
    template <typename T>
    BsonWriter& convert(const char*key, const T& data, x_for_class(T, int) *p=0) {
        (void)p;
    #endif
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

#endif
