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

#ifndef __X_JSON_READER_H
#define __X_JSON_READER_H

#include <stdexcept>
#include <fstream>

#include "rapidjson_custom.h"
#include "thirdparty/rapidjson/document.h"

#include "xreader.h"

namespace x2struct {


class JsonReader:public XReader<JsonReader> {
public:
    using xdoc_type::convert;

    JsonReader(const std::string& str, bool isfile=false):xdoc_type(0, ""),_doc(new rapidjson::Document),_val(_doc) {
        std::string err;
        std::string data;

        do {
            if (isfile) {
                std::ifstream fs(str.c_str(), std::ifstream::binary);
                if (!fs) {
                    err = "Open file["+str+"] fail.";
                    break;
                }
                std::string _tmp((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
                data.swap(_tmp);
                _doc->Parse(data);
            } else  {
                _doc->Parse(str);
            }

            if (_doc->HasParseError()) {
                size_t offset = _doc->GetErrorOffset();
                if  (isfile) {
                    std::string err_info = data.substr(offset, offset+32);
                    err = "Parse json file ["+str+"] fail. "+err_info;
                    break;
                } else {
                    std::string err_info = str.substr(offset, offset+32);
                    err = "Parse json string ["+str+"] fail. "+err_info;
                    break;
                }
            }
            init();
            return;
        } while (false);

        delete _doc;
        _doc = 0;
        throw std::runtime_error(err);
    }

    ~JsonReader() {
        if (0 != _doc) {
            delete _doc;
            _doc = 0;
        }
        if (0 != _iter) {
            delete _iter;
            _iter = 0;
        }
    }
public: // convert
    void convert(std::string &val) {
        val = _val->GetString();
    }
    void convert(bool &val) {
        val = _val->GetBool();
    }
    void convert(int16_t &val) {
        val = (int16_t)_val->GetInt();
    }
    void convert(uint16_t &val) {
        val = (uint16_t)_val->GetUint();
    }
    void convert(int32_t &val) {
        val = _val->GetInt();
    }
    void convert(uint32_t &val) {
        val = _val->GetUint();
    }
    void convert(int64_t &val) {
        val = _val->GetInt64();
    }
    void convert(uint64_t &val) {
        val = _val->GetUint64();
    }
    void convert(double &val) {
        val = _val->GetDouble();
    }
    void convert(float &val) {
        val = _val->GetFloat();
    }

    const std::string& type() {
        static std::string t("json");
        return t;
    }
    bool has(const char*key) {
        if (_val->HasMember(key)) {
            return !(*_val)[key].IsNull();
        } else {
            return false;
        }
    }
    size_t size(bool to_vec=true) {
        if (_val->IsArray()) {
            return (size_t)_val->Size();
        } else {
            return 0;
        }
    }
    JsonReader operator[](const char *key) {
        if (_val->HasMember(key)) {
            return JsonReader(&(*_val)[key], this, key);
        } else {
            throw std::runtime_error(std::string("Did not have ")+key);
        }
        return JsonReader(0, 0, "");
    }
    JsonReader operator[](size_t index) {
        if (_val->IsArray()) {
            return JsonReader(&(*_val)[(rapidjson::SizeType)index], this, index);
        } else {
            throw std::runtime_error("Out of index");
        }
        return JsonReader(0, 0, "");
    }
    JsonReader begin() {
        if (_iter != 0) {
            delete _iter;
        }
        _iter = new(rapidjson::Value::ConstMemberIterator);
        *_iter = _val->MemberBegin();
        if (*_iter != _val->MemberEnd()) {
            return JsonReader(&(*_iter)->value, this, (*_iter)->name.GetString());
        } else {
            return JsonReader(0, this, "");
        }
    }
    JsonReader next() {
        if (0 == _parent) {
            throw std::runtime_error("parent null");
        } else if (0 == _parent->_iter) {
            throw std::runtime_error("parent no iter");
        } else {
            ++(*_parent->_iter);
        }
        if (*_parent->_iter != _parent->_val->MemberEnd()) {
            return JsonReader(&(*_parent->_iter)->value, _parent, (*_parent->_iter)->name.GetString());
        } else {
            return JsonReader(0, _parent, "");
        }
    }
    operator bool() const {
        return 0 != _val;
    }

private:
    JsonReader(const rapidjson::Value* val, const JsonReader*parent, const char*key):xdoc_type(parent, key),_doc(0),_val(val) {
        init();
    }
    JsonReader(const rapidjson::Value* val, const JsonReader*parent, size_t index):xdoc_type(parent, index),_doc(0),_val(val) {
        init();
    }
    void init() {
        _iter = 0;
    }

    rapidjson::Document* _doc;
    const rapidjson::Value* _val;
    mutable rapidjson::Value::ConstMemberIterator* _iter;
};

}

#endif
