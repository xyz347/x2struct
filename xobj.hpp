#ifndef __X_TO_STRUCT_OBJ_HPP
#define __X_TO_STRUCT_OBJ_HPP

#include <stdint.h>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <stdexcept>
#include <boost/lexical_cast.hpp>

namespace x2struct {

class XObj {
public:
    XObj(const std::string&type):_type(type){}
    virtual ~XObj(){}
public:
    virtual void me_exception(const std::string&key);
    virtual void exception(const std::exception& e) {throw e;}
    virtual std::string hasa(const std::string&oname, const std::string&aname, bool &me); // alias name
public: // convert
    virtual void convert(std::string &val) = 0;
    virtual void convert(bool &val) = 0;
    virtual void convert(int16_t &val) = 0;
    virtual void convert(uint16_t &val) = 0;
    virtual void convert(int32_t &val) = 0;
    virtual void convert(uint32_t &val) = 0;
    virtual void convert(int64_t &val) = 0;
    virtual void convert(uint64_t &val) = 0;
    virtual void convert(double &val) = 0;
    virtual void convert(float &val) = 0;

    template <typename TYPE>
    void convert(std::vector<TYPE> &val) {
        for (size_t i=0; i<size(); ++i) {
            TYPE _t;
            this->subobj(i).convert(_t);
            val.push_back(_t);
        }
    }

    template <typename TYPE>
    void convert(std::map<std::string,TYPE> &val) {
        std::vector<std::string> fields;
        getsubfields(fields);
        for (size_t i=0; i<fields.size(); ++i) {
            TYPE _t;
            this->subobj(fields[i]).convert(_t);
            val[fields[i]] = _t;
        }
    }

    template <typename KEYTYPE, typename TYPE>
    void convert(std::map<KEYTYPE, TYPE> &val) {
        std::vector<std::string> fields;
        getsubfields(fields);
        for (size_t i=0; i<fields.size(); ++i) {
            TYPE _t;
            this->subobj(fields[i]).convert(_t);
            KEYTYPE _k = boost::lexical_cast<KEYTYPE>(fields[i]);
            val[_k] = _t;
        }
    }

    template <typename TYPE>
    void convert(std::set<TYPE> &val) {
        for (size_t i=0; i<size(); ++i) {
            TYPE _t;
            this->subobj(i).convert(_t);
            val.insert(_t);
        }
    }

    template <typename TYPE>
    void convert(TYPE& val) {
        size_t len = size();
        if (0==len) {
            val.__x_to_struct(*this);
        } else {
            for (size_t i=0; i<len; ++i) {
                XObj& sub = this->subobj(i);
                if (val.__x_condition(sub, _key.c_str())) {
                    val.__x_to_struct(sub);
                    break;
                }
            }
        }
    }
protected:
    std::string _path;
    void set_path(const std::string&father, const std::string&key);
    void set_path(const std::string&father, size_t index);
public:
    virtual bool has(const std::string&key) = 0;
    virtual size_t size() = 0;
    virtual XObj& operator[](const std::string&key) = 0;
    virtual XObj& subobj(size_t index) = 0;
    virtual XObj& subobj(const std::string&key) = 0;
    virtual void getsubfields(std::vector<std::string>& fields) = 0;
public:
    virtual std::string attribute(const std::string&key) = 0;   // xml有attribute, json/bson则获取普通的key即可
public:
    std::string _key;   // xml/json/bson基本都是key-value对，这里是key的名称
private:
    std::string _type;  // xml/json/bson/config
};

}

#endif


