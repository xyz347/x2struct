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

namespace x2struct {

class XmlWriter {
private:
    class XmlKey{
    public:
        XmlKey(const char*key, XmlWriter*writer, bool base);
        ~XmlKey();
    private:
        const char*_key;
        XmlWriter* _writer;
        bool _base;
        int  _lines;
    };
    friend class XmlKey;
public:
    XmlWriter(int identCount=0, char identChar=' ');
    ~XmlWriter();
public:
    std::string toStr();

    void array_begin();
    void array_end();
    void object_begin();
    void object_end();
    const std::string&type();

    XmlWriter& convert(const char*key, const std::string &val);
    XmlWriter& convert(const char*key, bool val);
    XmlWriter& convert(const char*key, int16_t val);
    XmlWriter& convert(const char*key, uint16_t val);
    XmlWriter& convert(const char*key, int32_t val);
    XmlWriter& convert(const char*key, uint32_t val);
    XmlWriter& convert(const char*key, int64_t val);
    XmlWriter& convert(const char*key, uint64_t val);
    XmlWriter& convert(const char*key, double val);
    XmlWriter& convert(const char*key, float val);

    template<typename T>
    XmlWriter& convert(const char*key, const std::vector<T>&data) {
        key=key[0]=='\0'?"x":key;
        this->array_begin();
        for (size_t i=0; i<data.size(); ++i) {
            XmlKey xkey(key, this, true);
            this->convert("", data[i]);
        }
        this->array_end();
        return *this;
    }

    template<typename T>
    XmlWriter& convert(const char*key, const std::set<T>&data) {
        key=key[0]=='\0'?"x":key;
        this->array_begin();
        for (typename std::set<T>::const_iterator it=data.begin(); it!=data.end(); ++it) {
            XmlKey xkey(key, this, true);
            this->convert("", *it);
        }
        this->array_end();
        return *this;
    }

    template <typename T>
    void convert(const char*key, const std::map<std::string, T> &data) {
        XmlKey xkey(key, this, false);
        this->object_begin();
        for (typename std::map<std::string,T>::const_iterator iter=data.begin(); iter!=data.end(); ++iter) {
            this->convert(iter->first, iter->second);
        }
        this->object_end();
    }

    template <typename KEY, typename T>
    void convert(const char*key, const std::map<KEY, T> &data) {
        XmlKey xkey(key, this, false);
        this->object_begin();
        for (typename std::map<KEY,T>::const_iterator iter=data.begin(); iter!=data.end(); ++iter) {
            std::string _k("x");
            _k.append(tostr(iter->first));
            this->convert(_k.c_str(), iter->second);
        }
        this->object_end();
    }

    template <typename T>
    void convert(const char*key, const T& data) {
        XmlKey xkey(key, this, false);
        this->object_begin();
        data.__struct_to_str(*this, key);
        this->object_end();
    }

    template <typename T>
    void convert(const char*key, const XType<T>& data) {
        data.__struct_to_str(*this, key);
    }

private:
    void append(const char* str, int len);
    void append(const std::string&str);
    void append(char ch);
    void indent();
    bool in_array();
    int  _indentCount;
    char _indentChar;
    int  _lines;
    std::vector<std::string> _buffer;
    std::string *_cur;
    std::vector<int> _state;
};

}


