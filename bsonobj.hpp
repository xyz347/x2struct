#ifndef __X_TO_STRUCT_BSON_OBJ_HPP
#define __X_TO_STRUCT_BSON_OBJ_HPP

#include "xobj.hpp"

//#include <iostream>
//using namespace std;

namespace mongo{
class BSONObj;
class BSONElement;
}

namespace x2struct {

// 差点重名了，就差大小写了
class BsonObj:public XObj {
public:
    BsonObj(const mongo::BSONObj&bobj);
    BsonObj(const mongo::BSONElement&bele);
    ~BsonObj();
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
    size_t size(bool&isarray);
private:
    const mongo::BSONObj& _v;
    const mongo::BSONElement& _e;
    bool  _isobj;
    std::vector<BsonObj> _gc;   //垃圾回收
    std::vector<mongo::BSONElement> _ec;   //垃圾回收
};

}

#endif


