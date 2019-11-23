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

#ifndef __X_CONFIG_READER_H
#define __X_CONFIG_READER_H

#include <stdexcept>
#include <sstream>

#include "thirdparty/libconfig/include/libconfig.h++"

#include "xreader.h"

namespace x2struct {

class ConfigReader:public XReader<ConfigReader> {
    typedef libconfig::Config  CONFIG_READER_DOCUMENT;
    typedef libconfig::Setting CONFIG_READER_VALUE;
    typedef libconfig::SettingConstIterator CONFIG_READER_ITERATOR;
public:
    friend class XReader<ConfigReader>;
    using xdoc_type::convert;

    ConfigReader(const std::string& str, bool isfile=false, const std::string&root=""):xdoc_type(0, ""),_doc(new CONFIG_READER_DOCUMENT),_val(0) {
        std::stringstream err;
        try {
            if (isfile) {
                _doc->readFile(str.c_str());
            } else {
                _doc->readString(str);
            }
            _val = &_doc->getRoot();
            if (root.empty()) {
                _val = &((*_val)[0]);
            } else {
                _val = &((*_val)[root]);
            }
            init();
            return;
        } catch (const libconfig::FileIOException &e) {
            err<<"load file["<<str<<"] failed:"<<e.what();
        } catch (const libconfig::ParseException &e) {
            if (isfile) {
                err<<"parse ["<<str<<"] error at ["<<e.getFile()<<":"<<e.getLine()<<" errinfo:"<<e.getError();
            } else {
                err<<"parse ["<<str<<"] error at line "<<e.getLine()<<" errinfo:"<<e.getError();
            }
        } catch (...) {
            err<<"Unknow exception when load["<<str<<"]";
        }

        delete _doc;
        _doc = 0;
        throw std::runtime_error(err.str());
    }
    ~ConfigReader() {
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
    #define XTOSTRUCT_CONFIG_GETVAL(...)                \
        const CONFIG_READER_VALUE *v = get_val(key);    \
        if (NULL != v) {                                \
            val = __VA_ARGS__(*v);                      \
            return true;                                \
        } else return false
    bool convert(const char *key, std::string &val) {
        const CONFIG_READER_VALUE *v = get_val(key);
        if (NULL != v) {
            val = v->c_str();
            return true;
        } else {
            return false;
        }
    }
    bool convert(const char *key, bool &val) {
        XTOSTRUCT_CONFIG_GETVAL();
    }
    bool convert(const char *key, int16_t &val) {
        XTOSTRUCT_CONFIG_GETVAL((int16_t)(int));
    }
    bool convert(const char *key, uint16_t &val) {
        XTOSTRUCT_CONFIG_GETVAL((uint16_t)(int));
    }
    bool convert(const char *key, int32_t &val) {
        XTOSTRUCT_CONFIG_GETVAL();
    }
    bool convert(const char *key, uint32_t &val) {
        XTOSTRUCT_CONFIG_GETVAL();
    }
    bool convert(const char *key, int64_t &val) {
        XTOSTRUCT_CONFIG_GETVAL();
    }
    bool convert(const char *key, uint64_t &val) {
        XTOSTRUCT_CONFIG_GETVAL();
    }
    bool convert(const char *key, double &val) {
        XTOSTRUCT_CONFIG_GETVAL();
    }
    bool convert(const char *key, float &val) {
        XTOSTRUCT_CONFIG_GETVAL();
    }

    const std::string& type() {
        static std::string t("config");
        return t;
    }
    bool has(const char*key) {
        return _val->exists(key);
    }
    size_t size(bool to_vec=true) {
        (void)to_vec;
        if (_val->isList()) {
            return (size_t)_val->getLength();
        } else {
            return 0;
        }
    }
    ConfigReader operator[](const char *key) {
        if (_val->exists(key)) {
            return ConfigReader(&(*_val)[key], this, key);
        } else {
            throw std::runtime_error(std::string("Did not have ")+key);
        }
        return ConfigReader(0, 0, "");
    }
    ConfigReader operator[](size_t index) {
        if (_val->isList()) {
            return ConfigReader(&(*_val)[index], this, index);
        } else {
            throw std::runtime_error("Out of index");
        }
        return ConfigReader(0, 0, "");
    }
    ConfigReader begin() {
        if (_iter != 0) {
            delete _iter;
        }

        _iter = new CONFIG_READER_ITERATOR(_val->begin());
        if (*_iter != _val->end()) {
            const CONFIG_READER_VALUE * s = _iter->operator->();
            return ConfigReader(s, this, s->getName());
        } else {
            return ConfigReader(0, this, "");
        }
    }
    ConfigReader next() {
        if (0 == _parent) {
            throw std::runtime_error("parent null");
        } else if (0 == _parent->_iter) {
            throw std::runtime_error("parent no iter");
        } else {
            ++(*_parent->_iter);
        }
        if (*_parent->_iter != _parent->_val->end()) {
            const CONFIG_READER_VALUE * s = _parent->_iter->operator->();
            return ConfigReader(s, _parent, s->getName());
        } else {
            return ConfigReader(0, _parent, "");
        }
    }
    operator bool() const {
        return 0!=_val;
    }

    void exception(const std::exception& e) {
        std::stringstream err;
        if (0 != dynamic_cast<const libconfig::FileIOException*>(&e)) {
            err<<"load file failed: "<<e.what();
        } else if (0 != dynamic_cast<const libconfig::ParseException*>(&e)) {
            const libconfig::ParseException*ne = dynamic_cast<const libconfig::ParseException*>(&e);
            err<<"parse config fail at "<<ne->getFile()<<":"<<ne->getLine()<<". error["<<ne->getError()<<"]";
        } else if (0 != dynamic_cast<const libconfig::SettingNotFoundException*>(&e)) {
            const libconfig::SettingNotFoundException*ne = dynamic_cast<const libconfig::SettingNotFoundException*>(&e);
            err<<"SettingNotFoundException: "<<ne->getPath();
        } else if (0 != dynamic_cast<const libconfig::SettingTypeException*>(&e)) {
            const libconfig::SettingTypeException*ne = dynamic_cast<const libconfig::SettingTypeException*>(&e);
            err<<"SettingTypeException: "<<ne->getPath();
        } else {
            err<<"Unknow exception:"<<e.what();
        }
        throw std::runtime_error(err.str());
    }

private:
    ConfigReader():xdoc_type(0, ""),_doc(0),_val(0) {
        init();
    }
    ConfigReader(const CONFIG_READER_VALUE* val, const ConfigReader*parent, const char*key):xdoc_type(parent, key),_doc(0),_val(val) {
        init();
    }
    ConfigReader(const CONFIG_READER_VALUE* val, const ConfigReader*parent, size_t index):xdoc_type(parent, index),_doc(0),_val(val) {
        init();
    }
    void init() {
        _iter = 0;
    }

    ConfigReader* child(const char*key, ConfigReader*tmp) {
        if (NULL!=_val && _val->exists(key)) {
            tmp->_key = key;
            tmp->_parent = this;
            tmp->_set_has = this->_set_has;
            tmp->_val = &(*_val)[key];
            return tmp;
        } else {
            return NULL;
        }
    }

    const CONFIG_READER_VALUE* get_val(const char *key) {
        if (NULL == key) {
            return _val;
        } else if (NULL != _val &&  _val->exists(key)) {
            return &(*_val)[key];
        } else {
            return NULL;
        }
    }

    CONFIG_READER_DOCUMENT* _doc;
    const CONFIG_READER_VALUE* _val;
    mutable CONFIG_READER_ITERATOR* _iter;
};

}

#endif
