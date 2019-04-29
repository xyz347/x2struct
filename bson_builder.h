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


#ifndef __X_BSON_BUILDER_H
#define __X_BSON_BUILDER_H

#include "xstdint.h"
#include <string>
#include <map>
#include <vector>
#include <set>

#include "thirdparty/libbson/include/libbson-1.0/bson.h"

#include "util.h"

struct _bson_t;

namespace bb { // not x2struct, short namespace for easy use

class intf;

typedef std::pair<std::string, intf> pintf;
typedef std::vector<int32_t> vi32;
typedef std::vector<int64_t> vi64;
typedef std::vector<std::string> vs;
typedef std::vector<pintf> vp;

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
    friend class Convert;
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
    intf(const vp& v):_type(t_mi),_vp(v){}

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
    vp  _vp;
};

class Convert {
public:
    enum {
        bb_top,
        bb_doc,
        bb_array
    };

    static std::string build(const vp&m, _bson_t*parent, const std::string&pname, int type) {
        bson_t _static;
        bson_t *root = &_static;

        if (type == bb_top) {
            if (0 != parent) {
                root = parent;
            } else {
                root = &_static;
            }
            bson_init(root);
        } else if (type == bb_doc) {
            bson_append_document_begin((bson_t*)parent, pname.c_str(), pname.length(), root);
        } else {
            bson_append_array_begin((bson_t*)parent, pname.c_str(), pname.length(), root);
        }

        for (vp::const_iterator iter=m.begin(); iter!=m.end(); ++iter) {
            switch (iter->second._type) {
              case intf::t_i32:
                bson_append_int32(root, iter->first.c_str(), iter->first.length(), iter->second._i64);
                break;
              case intf::t_i64:
                bson_append_int64(root, iter->first.c_str(), iter->first.length(), iter->second._i64);
                break;
              case intf::t_dt:
                bson_append_date_time(root, iter->first.c_str(), iter->first.length(), iter->second._i64);
                break;
              case intf::t_bool:
                bson_append_bool(root, iter->first.c_str(), iter->first.length(), iter->second._i64);
                break;
              case intf::t_double:
                bson_append_double(root, iter->first.c_str(), iter->first.length(), iter->second._db);
                break;
              case intf::t_vi32: {
                    bson_t child;
                    bson_append_array_begin(root, iter->first.c_str(), iter->first.length(), &child);
                    for (size_t i=0; i<iter->second._vi64.size(); ++i) {
                        std::string index = x2struct::Util::tostr(i);
                        bson_append_int32(&child, index.c_str(), index.length(), iter->second._vi64[i]);
                    }
                    bson_append_array_end(root, &child);
                }
                break;
              case intf::t_vi64: {
                    bson_t child;
                    bson_append_array_begin(root, iter->first.c_str(), iter->first.length(), &child);
                    for (size_t i=0; i<iter->second._vi64.size(); ++i) {
                        std::string index = x2struct::Util::tostr(i);
                        bson_append_int64(&child, index.c_str(), index.length(), iter->second._vi64[i]);
                    }
                    bson_append_array_end(root, &child);
                }
                break;
              case intf::t_vdouble: {
                    bson_t child;
                    bson_append_array_begin(root, iter->first.c_str(), iter->first.length(), &child);
                    for (size_t i=0; i<iter->second._vdb.size(); ++i) {
                        std::string index = x2struct::Util::tostr(i);
                        bson_append_double(&child, index.c_str(), index.length(), iter->second._vdb[i]);
                    }
                    bson_append_array_end(root, &child);
                }
                break;
              case intf::t_s:
                bson_append_utf8(root, iter->first.c_str(), iter->first.length(), iter->second._s.c_str(), iter->second._s.length());
                break;
              case intf::t_vs: {
                    bson_t child;
                    bson_append_array_begin(root, iter->first.c_str(), iter->first.length(), &child);
                    for (size_t i=0; i<iter->second._vs.size(); ++i) {
                        std::string index = x2struct::Util::tostr(i);
                        bson_append_utf8(&child, index.c_str(), index.length(), iter->second._vs[i].c_str(), iter->second._vs[i].length());
                    }
                    bson_append_array_end(root, &child);
                }
                break;
              case intf::t_mi:
                build(iter->second._vp, root, iter->first, bb_doc);
                break;
            }
        }

        if (type == bb_top) {
            if (0 != parent) {
                return "";
            } else {
                std::string data((const char*)bson_get_data(root), root->len);
                bson_destroy(root);
                return data;
            }
        } else if (type == bb_doc) {
            bson_append_document_end((bson_t*)parent, root);
        } else {
            bson_append_array_end((bson_t*)parent, root);
        }

        return "";
    }
    static void json(const vp&m, const std::string& space, std::string&root) {
        root.append("{").append(space);

        for (vp::const_iterator iter=m.begin(); iter!=m.end(); ++iter) {
            if (iter != m.begin()) {
                root.append(space).append(",").append(space);
            }
            if (!iter->first.empty()) {
                root.append("\"").append(iter->first).append("\"").append(space).append(":").append(space);
            }
            switch (iter->second._type) {
              case intf::t_i32:
              case intf::t_i64:
              case intf::t_dt:
                root.append(x2struct::Util::tostr(iter->second._i64));
                break;
              case intf::t_bool:
                root.append(iter->second._i64?"true":"false");
                break;
              case intf::t_double:
                root.append(x2struct::Util::tostr(iter->second._db));
                break;
              case intf::t_vi32:
              case intf::t_vi64:{
                    root.append("[").append(space);
                    for (size_t i=0; i<iter->second._vi64.size(); ++i) {
                        if (i > 0) {
                            root.append(space).append(",").append(space);
                        }
                        root.append(x2struct::Util::tostr(iter->second._vi64[i]));
                    }
                    root.append(space).append("]");
                }
                break;
              case intf::t_vdouble: {
                    root.append("[").append(space);
                    for (size_t i=0; i<iter->second._vdb.size(); ++i) {
                        if (i > 0) {
                            root.append(space).append(",").append(space);
                        }
                        root.append(x2struct::Util::tostr(iter->second._vdb[i]));
                    }
                    root.append(space).append("]");
                }
                break;
              case intf::t_s:
                root.append("\"").append(iter->second._s).append("\"");
                break;
              case intf::t_vs: {
                    root.append("[").append(space);
                    for (size_t i=0; i<iter->second._vs.size(); ++i) {
                        if (i > 0) {
                            root.append(space).append(",").append(space);
                        }
                        root.append("\"").append(iter->second._vs[i]).append("\"");
                    }
                    root.append(space).append("]");
                }
                break;
              case intf::t_mi:
                json(iter->second._vp, space, root);
                break;
            }
        }

        root.append(space).append("}");
    }
};

/*
if ret is nil, data will return as std::string
if ret is not nil, data will return as bson_t* and !!!need to call bson_destroy(ret)!!!, std::string will be empty
*/
//std::string build(const vp&m, _bson_t**ret, void*parent=0, const std::string&pname="", bool bdoc=false);
inline std::string build(const vp&m, _bson_t*ret) {
    return Convert::build(m, ret, "", Convert::bb_top);
}

// translate vp to json string
inline std::string json(const vp&m, bool space=false) {
    std::string root;
    Convert::json(m, space?" ":"", root);
    return root;
}

}

#endif
