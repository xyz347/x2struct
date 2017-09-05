#ifndef __X_TO_STRUCT_TYPES_HPP
#define __X_TO_STRUCT_TYPES_HPP

#include <stdint.h>
#include "xobj.hpp"

namespace x2struct {

class XType {
public:
    void __x_to_struct(XObj& obj) {
        std::string str;
        obj.convert(str);
        this->from_string(str);
    }
    bool __x_condition(XObj& obj, const std::string&name) {
        (void)obj;
        (void)name;
        return true;
    }
    template <class CLASS>
    void __struct_to_str(CLASS& obj, const std::string&name, int splen) const {
        std::string str = this->to_string();
        obj.convert(name, str, splen, 0);
    }
public:
    virtual std::string to_string() const = 0;
    virtual void from_string(const std::string&str) = 0;
};

class XDate:public XType{
public:
    int64_t unix_time;   // unix time
public:
    virtual std::string to_string() const;
    virtual void from_string(const std::string&str);
};

}

#endif


