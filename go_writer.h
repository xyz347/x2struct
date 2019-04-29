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

#ifndef __X_GO_WRITER_H
#define __X_GO_WRITER_H


#include "xstdint.h"
#include <string>
#include <map>
#include <vector>
#include <set>
#include <sstream>

#include <cxxabi.h>

#include "util.h"

namespace x2struct {

class GoCode {
public:
    GoCode(std::set<std::string>& alls, bool json=true, bool bson=true, bool xml=true)
        :_sub_struct_names(alls), _json(json),_bson(bson),_xml(xml) {
        if (json) {
            _types.push_back("json");
        }
        if (bson) {
            _types.push_back("bson");
        }
        if (xml) {
            _types.push_back("xml");
        }
    }
    ~GoCode() {}
public:
    void set_type_id_name(const std::string& name) {
        _name = raw_name(name);
    }
    std::string str() {
        std::string code;
        for (size_t i=0; i<_sub_structs.size(); ++i) {
            code.append(_sub_structs[i]);
        }

        size_t max_var_len = 0;
        size_t max_type_len = 0;
        for (size_t i=0; i<_var_names.size(); ++i) {
            if (_var_names[i].length() > max_var_len) {
                max_var_len = _var_names[i].length();
            }
        }
        for (size_t i=0; i<_var_types.size(); ++i) {
            if (_var_types[i].length() > max_type_len) {
                max_type_len = _var_types[i].length();
            }
        }

        max_var_len = ((max_var_len+4)>>2)<<2;
        max_type_len = ((max_type_len+4)>>2)<<2;

        code.append("type ").append(_name).append(" struct {\n");
        for (size_t i=0; i<_var_names.size(); ++i) {
            code.append("\t").append(_var_names[i]).append(std::string(max_var_len-_var_names[i].length(), ' '));
            code.append(_var_types[i]).append(std::string(max_type_len-_var_types[i].length(), ' '));
            code.append(_marshal_string[i]).append("\n");
        }
        code.append("}\n\n");
        return code;
    }
public:
    template<typename TYPE>
    void convert(const TYPE&member, const char*member_name, const char*aliase_name=0) {
        new_member(member_name, type_name(member), aliase_name);
    }
    
private:
    template <typename TYPE>
    std::string type_name(const std::vector<TYPE>& v) {
        TYPE t;
        return std::string("[]").append(type_name(t));
    }
    template <typename TYPE>
    std::string type_name(const std::set<TYPE>& v) {
        TYPE t;
        return std::string("[]").append(type_name(t));
    }
    template <typename TYPE>
    std::string type_name(const TYPE& v) {
        std::string tname = raw_name(typeid(v).name());

        if (_sub_struct_names.find(tname) == _sub_struct_names.end()) {
            _sub_struct_names.insert(tname);
            GoCode obj(_sub_struct_names, _json, _bson, _xml);
            _sub_structs.push_back(v.__struct_to_go(obj));
        }
        return tname;
    }
    template <typename KEY, typename VALUE>
    std::string type_name(const std::map<KEY, VALUE>& m) {
        KEY k;
        VALUE v;
        return std::string("map[").append(type_name(k)).append("]").append(type_name(v));
    }

    #define GOLANG_BASE_TYPE_NAME(type)         \
    std::string type_name(const type##_t & v) { \
        return #type;                           \
    }
    GOLANG_BASE_TYPE_NAME(int16);
    GOLANG_BASE_TYPE_NAME(int32);
    GOLANG_BASE_TYPE_NAME(int64);
    GOLANG_BASE_TYPE_NAME(uint16);
    GOLANG_BASE_TYPE_NAME(uint32);
    GOLANG_BASE_TYPE_NAME(uint64);
    std::string type_name(const std::string& v) {
        return std::string("string");
    }
    std::string type_name(bool v) {
        return std::string("bool");
    }
private:
    std::string raw_name(const std::string& type_id_name) {
        char* raw = abi::__cxa_demangle(type_id_name.c_str(), 0, 0, 0);
        std::string str;
        if (0 == raw) {
            str = std::string(type_id_name);
        } else {
            str = std::string(raw);
            free(raw);
        }

        std::vector<std::string> raw_types;
        Util::split(raw_types, str, "::"); // remove name space
        str = raw_types[raw_types.size()-1];

        str[0] = std::toupper(str[0]);
        return str;
    }
    // 变量原始名， go类型的名称（已经处理了slice类型的） 别名
    void new_member(const char*var_name, const std::string&go_type_name, const char*aliase_name) {
        std::string str(var_name);
        str[0] = std::toupper(str[0]);
        _var_names.push_back(str);

        _var_types.push_back(go_type_name);

        std::map<std::string, std::string> aliases;
        if (0 != aliase_name) {
            str = aliase_name;
            std::vector<std::string> types;
            Util::split(types, str, ' ');
            for (size_t i=0; i<types.size(); ++i) {
                std::vector<std::string> name_opts;
                Util::split(name_opts, types[i], ':');
                if (1 == name_opts.size()) {
                    aliases[""] = types[i];
                } else {
                    std::vector<std::string> opts;
                    Util::split(opts, name_opts[1], ',');
                    aliases[name_opts[0]] = opts[0];
                }
            }
        } else {
            aliases[""] = var_name;
        }

        str = "`";
        for (size_t i=0; i<_types.size(); ++i) {
            std::map<std::string,std::string>::const_iterator iter = aliases.find(_types[i]);
            std::string aname;
            if (aliases.end() == iter) {
                iter = aliases.find("");
            }
            if (iter == aliases.end()) {
                aname = var_name;
            } else {
                aname = iter->second;
            }
            
            if (i != 0) {
                str.append(" ");
            }
            str.append(_types[i]).append(":\"").append(aname).append("\"");
        }
        str.append("`");
        _marshal_string.push_back(str);
    }
private:
    std::set<std::string>& _sub_struct_names;   // 子结构体名称
    std::string _name;
    std::vector<std::string> _types;            // marshal需要哪些类型, json/bson/xml
    bool _json,_bson,_xml;
private:
    std::vector<std::string> _var_names;
    std::vector<std::string> _var_types;
    std::vector<std::string> _marshal_string;
private:
    std::vector<std::string> _sub_structs;      // 子结构体的代码
};

}

#endif


