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



#ifndef __X_TO_STRUCT_BSON_STR_HPP
#define __X_TO_STRUCT_BSON_STR_HPP

#include <stdint.h>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <boost/lexical_cast.hpp>
#include "mongo/bson/bson.h" // depend bson in include :(

#include "xstr.hpp"


namespace x2struct {

class BsonStr:public XStr {
public:
    BsonStr(mongo::BSONArrayBuilder* parent_array=0);
    ~BsonStr();
public:
    void begin(const std::string&root, int splen);
    void end(const std::string&root, int splen);
    int  space();
    std::string toStr() const;
    mongo::BSONObj toBson();
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
        mongo::BSONArrayBuilder array;
        for (size_t i=0; i<data.size(); ++i) {
            BsonStr tmp(&array);
            tmp.convert("", data[i], 0, 0);
            //array.append(tmp.toBson());
        }
                
        if (!name.empty()) {
            _bobj<<name<<array.arr();
        } else if (_parent_array!=0) {
            _parent_array->append(array.arr());
        } else {
            /* something wrong*/
        }
    }

    template <typename TYPE>
    void convert(const std::string&name, const std::set<TYPE> &data, int splen, int index) {
        mongo::BSONArrayBuilder array;
        for (auto iter=data.begin(); iter<data.end(); ++iter) {
            BsonStr tmp(array);
            tmp.convert("", *iter, 0, 0);
        }
                
        if (!name.empty()) {                                    
            _bobj<<name<<array.arr();                           
        } else if (_parent_array!=0) {                          
            _parent_array->append(array.arr());                   
        } else {                                                
            /* something wrong*/                                
        }
    }

    template <typename TYPE>
    void convert(const std::string&name, const std::map<std::string, TYPE> &data, int splen, int index) {
        mongo::BSONObjBuilder bdata;
        for (auto iter=data.begin(); iter!=data.end(); ++iter) {
            BsonStr tmp;
            tmp.convert(iter->first, iter->second, 0, 0);
            bdata.appendElements(tmp.toBson());
        }
        if (!name.empty()) {
            _bobj<<name<<bdata.obj();
        } else {
            _bobj.appendElements(bdata.obj());
        }
        //_bobj<<name<<bdata.obj();
    }

    template <typename KEYTYPE, typename TYPE>
    void convert(const std::string&name, const std::map<KEYTYPE, TYPE> &data, int splen, int index) {
        mongo::BSONObjBuilder bdata;
        for (auto iter=data.begin(); iter!=data.end(); ++iter) {
            BsonStr tmp;
            std::string _k = boost::lexical_cast<std::string>(iter->first);
            tmp.convert(_k, iter->second, 0, 0);
            bdata.appendElements(tmp.toBson());
        }
        if (!name.empty()) {
            _bobj<<name<<bdata.obj();
        } else {
            _bobj.appendElements(bdata.obj());
        }
        //_bobj<<name<<bdata.obj();
    }

    template <typename TYPE>
    void convert(const std::string&name, const TYPE& data, int splen, int index) {
        BsonStr tmp;
        data.__struct_to_str(tmp, "", 0);
        if (!name.empty()) {
            _bobj<<name<<tmp.toBson();
        } else if (_parent_array!=0) {
            _parent_array->append(tmp.toBson());
        } else {
            _bobj.appendElements(tmp.toBson());
        }
        //_bobj<<tmp.toBson();
    }
private:
    mongo::BSONObjBuilder _bobj;
    mongo::BSONArrayBuilder* _parent_array;
};

}

#endif


