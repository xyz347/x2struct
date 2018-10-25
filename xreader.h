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


#ifndef __X_READER_H
#define __X_READER_H


#include <stdint.h>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <stdexcept>
#include <iostream>
using namespace std;

#include "util.h"

namespace x2struct {

/*
  DOC need implement
  bool has(const std::string)
  const std::string& type()
*/
template<typename DOC>
class XReader {
protected:
    typedef DOC doc_type;
    typedef XReader<DOC> xdoc_type;
public:
    // only c++0x support reference initialize, so use pointer
    XReader(const doc_type *parent, const char* key):_parent(parent), _key(key), _index(-1){}
    XReader(const doc_type *parent, size_t index):_parent(parent), _key(0), _index(int(index)){}
    ~XReader(){}
public:
    template <typename TYPE>
    void convert(std::vector<TYPE> &val) {
        size_t s = static_cast<doc_type*>(this)->size();                // [implement] size_t size(bool to_vec=true)
        val.resize(s);
        for (size_t i=0; i<s; ++i) {
            (*static_cast<doc_type*>(this))[i].convert(val[i]);         // [implement] doc_type operator[](size_t)
        }
    }

    template <typename TYPE>
    void convert(std::set<TYPE> &val) {
        size_t s = static_cast<doc_type*>(this)->size();
        for (size_t i=0; i<s; ++i) {
            TYPE _t;
            (*static_cast<doc_type*>(this))[i].convert(_t);
            val.insert(_t);
        }
    }

    template <typename TYPE>
    void convert(std::map<std::string,TYPE> &val) {
        for (doc_type d=static_cast<doc_type*>(this)->begin(); d; d=d.next()) { // [implement] doc_type begin(); doc_type next(); operator bool() const;
            TYPE _t;
            d.convert(_t);
            val[d.key()] = _t;
        }
    }

    template <typename KEYTYPE, typename TYPE>
    void convert(std::map<KEYTYPE, TYPE> &val) {
        for (doc_type d=static_cast<doc_type*>(this)->begin(); d; d=d.next()) {
            TYPE _t;
            d.convert(_t);
            KEYTYPE _k;
            std::string key = d.key();
            if (key[0]!='x') {
                _k = Util::tonum<KEYTYPE>(key);
            } else { // libconfig/xml不支持数字作为key，所以用x开头，比如x11
                _k = Util::tonum<KEYTYPE>(key.substr(1));
            }
            val[_k] = _t;
        }
    }

    template <typename TYPE>
    void convert(TYPE& val) {
        size_t len = (static_cast<doc_type*>(this))->size(false);
        if (0==len) {
            val.__x_to_struct(*(static_cast<doc_type*>(this)));
        } else {
            for (size_t i=0; i<len; ++i) {
                doc_type sub = (*static_cast<doc_type*>(this))[i];
                if (val.__x_condition(sub, this->key_char())) {
                    val.__x_to_struct(sub);
                    break;
                }
            }
        }
    }

    std::string attribute(const char* key) {
        std::string val;
        (*static_cast<doc_type*>(this))[key].convert(val);
        return val;
    }

    const std::string key() const {
        if (0 != _key) {
            return _key;
        } else {
            return Util::tostr(_index);
        }
    }

    const char* key_char() const {
        if (0 != _key) {
            return _key;
        } else {
            return "";
        }
    }

    std::string hasa(const std::string&key, const std::string&alias, bool *me) {
        return Util::alias_parse(key, alias, static_cast<doc_type*>(this)->type(), me);
    }
    std::string path() {
        std::vector<std::string> nodes;
        const doc_type* tmp = static_cast<doc_type*>(this);
        while (tmp) {
            std::string k;
            k.reserve(32);
            if (0 != tmp->_key) {
                if (0!=tmp->_parent && 0!=tmp->_parent->_parent) {
                    k.append(".");
                }
                k.append(tmp->_key);
            } else {
                k.append("[").append(Util::tostr(tmp->_index)).append("]");
            }
            nodes.push_back(k);
            tmp = tmp->_parent;
        }
        std::string p;
        p.reserve(64);
        for (int i=(int)nodes.size()-1; i>=0; --i) {
            p.append(nodes[i]);
        }
        return p;
    }
    void me_exception(const std::string&key) {
        std::string err;
        err.reserve(128);
        err.append("miss ");
        std::string p = path();
        if (!p.empty()) {
            err.append(p).append(".");
        }
        err.append(key);
        throw std::runtime_error(err);
    }
protected:
    const doc_type* _parent;
    const char* _key;
    int _index;
};

}

#endif
