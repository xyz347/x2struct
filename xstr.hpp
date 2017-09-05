#ifndef __X_TO_STRUCT_STRING_HPP
#define __X_TO_STRUCT_STRING_HPP

#include <string>
#include <type_traits>

namespace x2struct {

class XStr {
public:
    XStr(const std::string&type):_type(type){}
    virtual ~XStr(){}
public:
    virtual std::string fieldName(const std::string&oname, const std::string&aname); // alias name
public:
    virtual void convert(const std::string&name, const std::string& data, int space, int index) = 0;
private:
    std::string _type;  // xml/json/bson/config
};

}

#endif


