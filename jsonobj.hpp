#ifndef __X_TO_STRUCT_JSON_OBJ_HPP
#define __X_TO_STRUCT_JSON_OBJ_HPP

#include "xobj.hpp"
//#include <json/json.h>
//#include <iostream>
//using namespace std;

namespace Json{
class Value;
}

namespace x2struct {

class JsonObj:public XObj {
public:
    JsonObj(const std::string& str, bool isfile=false);
    ~JsonObj();
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
    virtual void getsubfields(std::vector<std::string>& fields);
public:
    virtual std::string attribute(const std::string&key);
private:
    JsonObj(Json::Value& v);
private:
    Json::Value* __v;
    Json::Value& _v;
    std::vector<JsonObj> _gc;   //垃圾回收
};

}

#endif


