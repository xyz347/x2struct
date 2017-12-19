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

namespace bb { // not x2struct, short namespace for easy use

class intf;

typedef std::vector<int32_t> vi32;
typedef std::vector<int64_t> vi64;
typedef std::vector<std::string> vs;
typedef std::map<std::string, intf> mi;

// date time
struct dt {
    int64_t t;
    dt(int64_t ms):t(ms){}
};

class intf {
    enum {
        t_i32,
        t_i64,
        t_vi32,
        t_vi64,
        t_s,
        t_vs,
        t_mi,
        t_bool,
        t_dt,
        t_double,
        t_vdouble,
    };
    friend std::string build(const mi&m, _bson_t*parent, const std::string&pname, int type);
public:
    intf(bool v):_type(t_bool),_i64(v){}
    intf(int32_t v):_type(t_i32),_i64(v){}
    intf(int64_t v):_type(t_i64),_i64(v){}
    intf(const dt&v):_type(t_dt),_i64(v.t){}
    intf(const vi32& v):_type(t_vi32){
        _vi64.resize(v.size());
        for (size_t i=0; i<v.size(); ++i) {
            _vi64[i] = v[i];
        }
    }
    intf(const vi64& v):_type(t_vi64),_vi64(v){}
    intf(const std::string& v):_type(t_s),_s(v){}
    intf(const vs& v):_type(t_vs),_vs(v){}
    intf(const mi& v):_type(t_mi),_mi(v){}

    // fix type
    intf(const char* v):_type(t_s),_s(v){}
    intf(uint32_t v):_type(t_i32),_i64(v){}
    intf(uint64_t v):_type(t_i64),_i64(v){}
    intf(const std::vector<uint32_t>& v):_type(t_vi32){
        _vi64.resize(v.size());
        for (size_t i=0; i<v.size(); ++i) {
            _vi64[i] = v[i];
        }
    }
    intf(const std::vector<uint64_t>& v):_type(t_vi64){
        _vi64.resize(v.size());
        for (size_t i=0; i<v.size(); ++i) {
            _vi64[i] = v[i];
        }
    }
    intf(double v):_type(t_double),_db(v){}
    intf(float v):_type(t_double),_db(v){}
    intf(const std::vector<double>& v):_type(t_vdouble),_vdb(v){}
    intf(const std::vector<float>& v):_type(t_vdouble){
        _vdb.resize(v.size());
        for (size_t i=0; i<v.size(); ++i) {
            _vdb[i] = v[i];
        }
    }
private:
    int _type;
    int64_t _i64;
    std::string _s;
    vi64  _vi64;
    double _db;
    std::vector<double> _vdb;
    vs  _vs;
    mi  _mi;
};

/*
to build a bson binary data buffer, build(m, ret) is ok

if ret is nil, data will return as std::string
if ret is not nil, data will return as bson_t* and !!!need to call bson_destroy(ret)!!!, std::string will be empty
*/
//std::string build(const mi&m, _bson_t**ret, void*parent=0, const std::string&pname="", bool bdoc=false);
std::string build(const mi&m, _bson_t*ret);

}

