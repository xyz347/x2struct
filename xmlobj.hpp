#ifndef __X_TO_STRUCT_XML_OBJ_HPP
#define __X_TO_STRUCT_XML_OBJ_HPP

#include <map>
#include "xobj.hpp"
//#include <tinyxml.h>

class TiXmlElement;
class TiXmlDocument;

namespace x2struct {


class XmlObj:public XObj {
public:
    XmlObj(const std::string&str, bool isfile=true);
    ~XmlObj();
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
    XmlObj(const XmlObj& father, const std::string&key);
    XmlObj(const XmlObj& me, size_t index);
    void init_data();
private:
    const TiXmlElement* _ele;
    TiXmlDocument* _doc;
    std::map<std::string, std::string> _attribute;
    std::vector<const TiXmlElement*> _brothers;
    std::set<std::string> _childs;
    std::vector<std::string> _child_vects;
    std::vector<XmlObj> _gc;   //垃圾回收
};

}

#endif


