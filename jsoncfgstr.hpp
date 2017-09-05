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


#ifndef __X_TO_STRUCT_JSON_CFG_STR_HPP
#define __X_TO_STRUCT_JSON_CFG_STR_HPP

#include <stdint.h>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <sstream>
#include <boost/lexical_cast.hpp>

#include "xstr.hpp"

namespace x2struct {

class JsonCfgStr:public XStr {
public:
    JsonCfgStr(bool isjson, bool newline=false, int splen=4);
    ~JsonCfgStr();
public:
    void begin(const std::string&root, int splen);
    void end(const std::string&root, int splen);
    int  space();
    std::string toStr() const;
public:
    virtual void convert(const std::string&name, const std::string& data, int space, int index);

    // base type
    void convert(const std::string&name, int16_t data, int splen, int index);
    void convert(const std::string&name, uint16_t data, int splen, int index);
    void convert(const std::string&name, int32_t data, int splen, int index);
    void convert(const std::string&name, uint32_t data, int splen, int index);
    void convert(const std::string&name, int64_t data, int splen, int index);
    void convert(const std::string&name, uint64_t data, int splen, int index);
    void convert(const std::string&name, float data, int splen, int index);
    void convert(const std::string&name, double data, int splen, int index);
    void convert(const std::string&name, bool data, int splen, int index);

    // base type vector
    void convert(const std::string&name, const std::vector<int16_t>& data, int splen, int index);
    void convert(const std::string&name, const std::vector<uint16_t>& data, int splen, int index);
    void convert(const std::string&name, const std::vector<int32_t>& data, int splen, int index);
    void convert(const std::string&name, const std::vector<uint32_t>& data, int splen, int index);
    void convert(const std::string&name, const std::vector<int64_t>& data, int splen, int index);
    void convert(const std::string&name, const std::vector<uint64_t>& data, int splen, int index);
    void convert(const std::string&name, const std::vector<float>& data, int splen, int index);
    void convert(const std::string&name, const std::vector<double>& data, int splen, int index);
    void convert(const std::string&name, const std::vector<bool>& data, int splen, int index);
    void convert(const std::string&name, const std::vector<std::string>& data, int splen, int index);

    // base type set
    void convert(const std::string&name, const std::set<int16_t>& data, int splen, int index);
    void convert(const std::string&name, const std::set<uint16_t>& data, int splen, int index);
    void convert(const std::string&name, const std::set<int32_t>& data, int splen, int index);
    void convert(const std::string&name, const std::set<uint32_t>& data, int splen, int index);
    void convert(const std::string&name, const std::set<int64_t>& data, int splen, int index);
    void convert(const std::string&name, const std::set<uint64_t>& data, int splen, int index);
    void convert(const std::string&name, const std::set<float>& data, int splen, int index);
    void convert(const std::string&name, const std::set<double>& data, int splen, int index);
    void convert(const std::string&name, const std::set<bool>& data, int splen, int index);
    void convert(const std::string&name, const std::set<std::string>& data, int splen, int index);

    template <typename TYPE>
    void convert(const std::string&name, const std::vector<TYPE> &data, int splen, int index) {
        linefeed(index);
        std::string oldsep(_sep);
        //_ss<<std::string(splen, ' ')<<_key<<name<<_key<<_assign<<_vecb;
        if (!name.empty()) {
            _ss<<std::string(splen, ' ')<<_key<<name<<_key<<_assign<<_vecb;
        } else {
            _ss<<std::string(splen, ' ')<<_vecb;
        }
        for (size_t i=0; i<data.size(); ++i) {
            _sep=",";
            this->convert("", data[i], splen+space(), (int)i);
            _sep=oldsep;
        }
        _ss<<(_newline?"\n":"")<<std::string(splen, ' ')<<_vece;
    }

    template <typename TYPE>
    void convert(const std::string&name, const std::set<TYPE> &data, int splen, int index) {
        int i = 0;
        linefeed(index);
        std::string oldsep(_sep);
        //_ss<<std::string(splen, ' ')<<_key<<name<<_key<<_assign<<_vecb;
        if (!name.empty()) {
            _ss<<std::string(splen, ' ')<<_key<<name<<_key<<_assign<<_vecb;
        } else {
            _ss<<std::string(splen, ' ')<<_vecb;
        }
        for (typename std::set<TYPE>::const_iterator it=data.begin(); it!=data.end(); ++it) {
            _sep=",";
            this->convert("", *it, splen+space(), i++);
            _sep=oldsep;
        }
        _ss<<(_newline?"\n":"")<<std::string(splen, ' ')<<_vece;
    }

    template <typename TYPE>
    void convert(const std::string&name, const std::map<std::string,TYPE> &data, int splen, int index) {
        int i = 0;
        linefeed(index);
        std::string oldsep(_sep);
        if (!name.empty()) {
            _ss<<std::string(splen, ' ')<<_key<<name<<_key<<_assign<<"{";
        } else {
            _ss<<std::string(splen, ' ')<<"{";
        }
        for (auto iter=data.begin(); iter!=data.end(); ++iter) {
            _sep=",";
            this->convert(iter->first, iter->second, splen+space(), i++);
            _sep=oldsep;
        }
        _ss<<(_newline?"\n":"")<<std::string(splen, ' ')<<"}";
    }

    template <typename KEYTYPE, typename TYPE>
    void convert(const std::string&name, const std::map<KEYTYPE, TYPE> &data, int splen, int index) {
        int i = 0;
        linefeed(index);
        std::string oldsep(_sep);
        if (!name.empty()) {
            _ss<<std::string(splen, ' ')<<_key<<name<<_key<<_assign<<"{";
        } else {
            _ss<<std::string(splen, ' ')<<"{";
        }
        for (auto iter=data.begin(); iter!=data.end(); ++iter) {
            _sep=",";
            std::string _k = boost::lexical_cast<std::string>(iter->first);
            this->convert(_k, iter->second, splen+space(), i++);
            _sep=oldsep;
        }
        _ss<<(_newline?"\n":"")<<std::string(splen, ' ')<<"}";
    }

    template <typename TYPE>
    void convert(const std::string&name, const TYPE& data, int splen, int index) {
        linefeed(index);
        data.__struct_to_str(*this, name, splen);
    }
private:
    void linefeed(int index) {
        if (index>0) {
            _ss<<_sep;
        }
        if (_newline) {
            _ss<<'\n';
        }
    }
    template <typename TYPE>
    void base_convert(const std::string&name, const TYPE& data, int splen, int index) {
        linefeed(index);
        if (!name.empty()) {
            _ss<<std::string(splen, ' ')<<_key<<name<<_key<<_assign<<data;
        } else {
            _ss<<std::string(splen, ' ')<<data;
        }
    }
    template <typename TYPE>
    void base_vector(const std::string&name, const std::vector<TYPE>& data, int splen, int index) {
        linefeed(index);
        if (!name.empty()) {
            _ss<<std::string(splen, ' ')<<_key<<name<<_key<<_assign<<_vecb;
        } else {
            _ss<<std::string(splen, ' ')<<_vecb;
        }
        for (typename std::vector<TYPE>::const_iterator it=data.begin(); it!=data.end(); ++it) {
            if (it != data.begin()) {
                _ss<<',';
            }
            _ss<<*it;
        }
        _ss<<_vece;
    }
    template <typename TYPE>
    void base_set(const std::string&name, const std::set<TYPE>& data, int splen, int index) {
        linefeed(index);
        if (!name.empty()) {
            _ss<<std::string(splen, ' ')<<_key<<name<<_key<<_assign<<_vecb;
        } else {
            _ss<<std::string(splen, ' ')<<_vecb;
        }
        for (typename std::set<TYPE>::const_iterator it=data.begin(); it!=data.end(); ++it) {
            if (it != data.begin()) {
                _ss<<',';
            }
            _ss<<*it;
        }
        _ss<<_vece;
    }
private:
    bool _json;
    bool _newline;
    int  _space;
    std::string _vecb;  // json:[, cfg:(
    std::string _vece;  // json:], cfg:)
    std::string _key;   // json:", cfg:(empty)
    std::string _assign;// json::, cfg:=
    std::string _sep;   // json:,, cfg:;
    std::stringstream _ss;
};

}

#endif


