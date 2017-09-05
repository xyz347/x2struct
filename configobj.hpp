#ifndef __X_TO_STRUCT_LIBCONFIG_OBJ_HPP
#define __X_TO_STRUCT_LIBCONFIG_OBJ_HPP

#include "xobj.hpp"
//#include <json/json.h>
//#include <iostream>
//using namespace std;

namespace libconfig {
class Config;
class Setting;
class FileIOException;
class ParseException;
class SettingNotFoundException;
class SettingTypeException;
}

namespace x2struct {

class ConfigObj:public XObj {
public:
    ConfigObj(const std::string& str, bool isfile=false, const std::string&root="");
    ~ConfigObj();
public:
    virtual void exception(const std::exception& e);
public: // convert
    virtual void convert(std::string &val);
    virtual void convert(bool &val);
    virtual void convert(int16_t &val);
    virtual void convert(uint16_t &val);
    virtual void convert(int32_t &val);
    virtual void convert(uint32_t &val);
    virtual void convert(int64_t &val);
    virtual void convert(uint64_t &val);
    virtual void convert(double &val);
    virtual void convert(float &val);

public:
    virtual bool has(const std::string&key);
    virtual size_t size();
    virtual XObj& operator[](const std::string&key);
    virtual XObj& subobj(size_t index);
    virtual XObj& subobj(const std::string&key);
    virtual void getsubfields(std::vector<std::string>& fields) {} // config++ support this on 1.6
public:
    virtual std::string attribute(const std::string&key);
private:
    ConfigObj(libconfig::Setting& s);
private:
    libconfig::Config*  _c;
    libconfig::Setting* _s;
    std::vector<ConfigObj> _gc;   //垃圾回收
};

}

#endif


