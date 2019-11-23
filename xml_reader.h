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

#ifndef __X_XML_READER_H
#define __X_XML_READER_H

#include <map>
#include <vector>
#include <stdexcept>
#include <fstream>
//#include <iostream>

#include <string.h>

#include "thirdparty/rapidxml/rapidxml.hpp"

#include "xreader.h"


namespace x2struct {


// xml没有数组，适配起来有点恶心
class XmlReader:public XReader<XmlReader> {
    typedef rapidxml::xml_document<> XML_READER_DOCUMENT;
    typedef rapidxml::xml_node<> XML_READER_NODE;  
public:
    friend class XReader<XmlReader>;
    using xdoc_type::convert;
    XmlReader(const std::string& str, bool isfile=false):xdoc_type(0, ""),_doc(new XML_READER_DOCUMENT),_val(0),_siblings(0) {
        std::string err;
        _xml_data = 0;

        do {
            try {
                if (isfile) {
                    std::ifstream fs(str.c_str(), std::ifstream::binary);
                    if (!fs) {
                        err = "Open file["+str+"] fail.";
                        break;
                    }
                    std::string data((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
                    _xml_data = new char[data.length()+1];
                    memcpy(_xml_data, data.data(), data.length());
                    _xml_data[data.length()] = '\0';
                } else  {
                    _xml_data = new char[str.length()+1];
                    memcpy(_xml_data, str.data(), str.length());
                    _xml_data[str.length()] = '\0';
                }
                _doc->parse<0>(_xml_data);
            } catch (const rapidxml::parse_error&e) {
                err = std::string("parse error[")+e.what()+"] "+std::string(e.where<char>()).substr(0, 32);
            } catch (const std::exception&e) {
                err = std::string("unknow exception[")+e.what()+"]";
            }

            if (!err.empty()) {
                break;
            }
            _val = _doc->first_node();
            init();
            return;
        } while (false);

        delete _doc;
        _doc = 0;
        if (0 != _xml_data) {
            delete []_xml_data;
            _xml_data = 0;
        }
        throw std::runtime_error(err);
    }
    ~XmlReader() {
        if (0 != _doc) {
            delete _doc;
            delete []_xml_data;
            _doc = 0;
        }
    }
private:
    class NodeOrAttr {
    private:
        rapidxml::xml_attribute<char> *attr;
        const XML_READER_NODE* node;
    public:
        NodeOrAttr(rapidxml::xml_attribute<char> *a):attr(a),node(0){}
        NodeOrAttr(const XML_READER_NODE* n):attr(0),node(n){}
        NodeOrAttr():attr(0),node(0){}
        operator bool() const {
            return attr!=0 || node!=0;
        }
        std::string value() {
            if (node != 0) {
                return node->value();
            } else if (attr != 0) {
                return attr->value();
            } else {
                return "";
            }
        }
    };
public: // convert
    #define XTOSTRUCT_XML_GETVAL()      \
        NodeOrAttr v = get_val(key);    \
        if (v && !v.value().empty())
    bool convert(const char *key, std::string &val) {
        XTOSTRUCT_XML_GETVAL() {
            val = v.value();
            return true;
        } else {
            return false;
        }
    }
    bool convert(const char *key, bool &val) {
        XTOSTRUCT_XML_GETVAL() {
            std::string tmp=v.value();
            if (tmp=="1" || tmp=="true" || tmp=="TRUE" || tmp=="True") {
                val = true;
            } else {
                val = false;
            }
            return true;
        } else {
            val = false;
            return false;
        }
    }
    bool convert(const char *key, int16_t &val) {
        XTOSTRUCT_XML_GETVAL() {
            val = Util::tonum<int16_t>(v.value());
            return true;
        } else {
            return false;
        }
    }
    bool convert(const char *key, uint16_t &val) {
        XTOSTRUCT_XML_GETVAL() {
            val = Util::tonum<uint16_t>(v.value());
            return true;
        } else {
            return false;
        }
    }
    bool convert(const char *key, int32_t &val) {
        XTOSTRUCT_XML_GETVAL() {
            val = Util::tonum<int32_t>(v.value());
            return true;
        } else {
            return false;
        }
    }
    bool convert(const char *key, uint32_t &val) {
        XTOSTRUCT_XML_GETVAL() {
            val = Util::tonum<uint32_t>(v.value());
            return true;
        } else {
            return false;
        }
    }
    bool convert(const char *key, int64_t &val) {
        XTOSTRUCT_XML_GETVAL() {
            val = Util::tonum<int64_t>(v.value());
            return true;
        } else {
            return false;
        }
    }
    bool convert(const char *key, uint64_t &val) {
        XTOSTRUCT_XML_GETVAL() {
            val = Util::tonum<uint64_t>(v.value());
            return true;
        } else {
            return false;
        }
    }
    bool convert(const char *key, double &val) {
        XTOSTRUCT_XML_GETVAL() {
            val = Util::tonum<double>(v.value());
            return true;
        } else {
            return false;
        }
    }
    bool convert(const char *key, float &val) {
        XTOSTRUCT_XML_GETVAL() {
            val = Util::tonum<float>(v.value());
            return true;
        } else {
            return false;
        }
    }

    const std::string& type() {
        static std::string t("xml");
        return t;
    }
    bool has(const char*key) {
        if (_child_index.find(key)!=_child_index.end()) {
            return true;
        } else if (0 != _val->first_attribute(key)) {
            return true;
        } else {
            return false;
        }
    }
    size_t size(bool to_vec=true) {
        if (0 != _siblings) {
            return _siblings->size();
        } else if (_index>=0 && to_vec && _childs.size()==1) {   // 嵌套数组
            _siblings = &_childs[0];
            return _siblings->size();
        } else {
            return 0;
        }
    }
    XmlReader operator[](const char *key) {
        if (has(key)) {
            if (_child_index.find(key)!=_child_index.end()) {
                return XmlReader(&_childs[_child_index[key]], this, key);
            } else {
                return XmlReader(_siblings, this, key);
            }
        } else {
            throw std::runtime_error(std::string("Did not have ")+key);
        }
        return XmlReader(0, 0, "");
    }
    XmlReader operator[](size_t index) {
        if (0 != _siblings) {
            return XmlReader((*_siblings)[index], this, index);
        } else if (_index>=0 && index<_childs.size()) { //
            return XmlReader(&_childs[index], this, index);
        } else {
            throw std::runtime_error("Out of index");
        }
        return XmlReader(0, 0, "");
    }
    XmlReader begin() {
        _iter = _child_index.begin();
        if (_iter != _child_index.end()) {
            return XmlReader(&_childs[_iter->second], this, _iter->first);
        } else {
            return XmlReader(0, this, "");
        }
    }
    XmlReader next() {
        if (0 == _parent) {
            throw std::runtime_error("parent null");
        } else {
            ++(_parent->_iter);
        }
        if (_parent->_iter != _parent->_child_index.end()) {
            return XmlReader(&_parent->_childs[_parent->_iter->second], _parent, _parent->_iter->first);
        } else {
            return XmlReader(0, _parent, "");
        }
    }
    operator bool() const {
        return 0!=_val || 0!=_siblings;
    }

    std::string attribute(const char* key) { // overwite
        if (0 != _val) {
            rapidxml::xml_attribute<char> *attr = _val->first_attribute(key);
            if (0!=attr && attr->value()) {
                return attr->value();
            }
        }
        return "";
    }

private:
    typedef std::vector<XML_READER_NODE*> sibling_type;
    typedef std::vector<sibling_type> child_type;
    typedef std::map<const char*, size_t, cmp_str> child_index_type;

    XmlReader():xdoc_type(0, ""),_doc(0),_val(0),_siblings(0) {
        init();
    }
    XmlReader(const sibling_type* val, const XmlReader*parent, const char*key):xdoc_type(parent, key),_doc(0),_val(0),_siblings(val) {
        init();
    }
    XmlReader(const sibling_type* val, const XmlReader*parent, size_t index):xdoc_type(parent, index),_doc(0),_val(0),_siblings(val) {
        init();
    }
    XmlReader(const XML_READER_NODE* val, const XmlReader*parent, size_t index):xdoc_type(parent, index),_doc(0),_val(val),_siblings(0) {
        init();
    }
    void init() {
        if (0 != _siblings) {
            _val=(*_siblings)[0];
        }
        if (0 != _val) {
            XML_READER_NODE *tmp = _val->first_node();
            for (; tmp; tmp=tmp->next_sibling()) {
                child_index_type::iterator it = _child_index.find(tmp->name());
                if (it != _child_index.end()) {
                    _childs[it->second].push_back(tmp);
                } else {
                    sibling_type _v;
                    _v.push_back(tmp);
                    _childs.push_back(_v);
                    _child_index[tmp->name()] = _childs.size()-1;
                }
            }
        }
    }
    
    XmlReader* child(const char*key, XmlReader*tmp) {
        child_index_type::iterator iter;
        if (_child_index.end()!=(iter=_child_index.find(key))) {
            tmp->_key = key;
            tmp->_parent = this;
            tmp->_set_has = this->_set_has;
            tmp->_siblings = &_childs[iter->second];
            tmp->init();
            return tmp;
        } else {
            return NULL;
        }
    }

    NodeOrAttr get_val(const char *key) {
        if (NULL == key) {
            return _val;
        } else {
            child_index_type::iterator iter;
            if (_child_index.end()!=(iter=_child_index.find(key))) {
                return _childs[iter->second][0];
            } else if (0 != _val) {
                rapidxml::xml_attribute<char> *attr = _val->first_attribute(key);
                return attr;
            } else {
                return NodeOrAttr();
            }
        }
    }

    XML_READER_DOCUMENT* _doc;
    char *_xml_data;

    const XML_READER_NODE* _val;
    const sibling_type *_siblings;
    child_type _childs;
    child_index_type _child_index; // 记录某个key在_childs中的索引
    mutable child_index_type::iterator _iter;
};

}

#endif
