#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include <tinyxml.h>
#include "xmlobj.hpp"
#include <iostream>

namespace x2struct {

using namespace ::std;

XmlObj::XmlObj(const std::string&str, bool isfile):XObj("xml"),_ele(0),_doc(0)
{
    string err;
    _doc = new TiXmlDocument;
    do {
        if (isfile) {
            FILE *fp = fopen(str.c_str(), "rb");
            if (!fp) {
                err = "Open file["+str+"] fail.";
                break;
            } else if (!_doc->LoadFile(fp)) {
                err = "Try to parse ["+str+"] fail.";
                fclose(fp);
                break;
            }
            fclose(fp);
        } else {
            _doc->Parse(str.c_str(), 0, TIXML_ENCODING_UTF8);
            if (0 != _doc->ErrorId()) {
                err = "Try to parse ["+str+"] fail:["+_doc->ErrorDesc()+"]";
                break;
            }
        }

        _ele = _doc->RootElement();
        _brothers.push_back(_ele);
        _key = _ele->Value();
        init_data();
        return;
    } while (false);

    delete _doc;
    _doc = 0;
    throw std::runtime_error(err);
}

XmlObj::XmlObj(const XmlObj& father, const std::string&key):XObj("xml"),_ele(0),_doc(0)
{
    _key = key;
    const TiXmlElement* c = father._ele->FirstChildElement();
    while (0 != c) {
        if (c->Value() == key) {
            _brothers.push_back(c);
            if (_brothers.size() == 1) {
                _ele = c;
            }
        }
        c = c->NextSiblingElement();
    }
    init_data();
}

XmlObj::XmlObj(const XmlObj&me, size_t index):XObj("xml"),_ele(me._brothers[index]),_doc(0)
{
    _key = me._key;
    _brothers.push_back(_ele);
    init_data();
}

XmlObj::~XmlObj()
{
    if (0 != _doc) {
        delete _doc;
        _doc = 0;
    }
}

void XmlObj::init_data()
{
    if (_brothers.size() == 1) {
        const TiXmlAttribute *attr = _ele->FirstAttribute();
        while (attr) {
            _attribute[attr->Name()] = attr->Value();
            attr = attr->Next();
        }
    }

    const TiXmlElement* c = _ele->FirstChildElement();
    while (0 != c) {
        _childs.insert(c->Value());
        _child_vects.push_back(c->Value());
        c = c->NextSiblingElement();
    }
}

void XmlObj::convert(std::string &val)
{
    val = _ele->GetText();
}

void XmlObj::convert(bool &val)
{
    std::string str(_ele->GetText());
    if (str=="true" || str=="True" || str=="TRUE" || str=="1") {
        val = true;
    } else {
        val = false;
    }
}

void XmlObj::convert(int32_t &val)
{
    val = boost::lexical_cast<int32_t>(_ele->GetText());
}

void XmlObj::convert(uint32_t &val)
{
    val = boost::lexical_cast<uint32_t>(_ele->GetText());
}

void XmlObj::convert(int16_t &val)
{
    val = boost::lexical_cast<int16_t>(_ele->GetText());
}

void XmlObj::convert(uint16_t &val)
{
    val = boost::lexical_cast<uint16_t>(_ele->GetText());
}

void XmlObj::convert(int64_t &val)
{
    val = boost::lexical_cast<int64_t>(_ele->GetText());
}

void XmlObj::convert(uint64_t &val)
{
    val = boost::lexical_cast<uint64_t>(_ele->GetText());
}

void XmlObj::convert(double &val)
{
    val = boost::lexical_cast<double>(_ele->GetText());
}

void XmlObj::convert(float &val)
{
    val = boost::lexical_cast<float>(_ele->GetText());
}


bool XmlObj::has(const std::string&key)
{
    return (_childs.find(key)!=_childs.end());
}

size_t XmlObj::size()
{
    return (size_t)_brothers.size();
}

XObj& XmlObj::operator[](const std::string&key)
{
    return subobj(key);
}

XObj& XmlObj::subobj(size_t index)
{
    if (index >= _brothers.size()) {
        std::string err = "Out of index";
        throw std::runtime_error(err);
    }

    XmlObj obj(*this, index);
    obj.set_path(_path, index);
    _gc.push_back(obj);
    return _gc[_gc.size()-1];
}

XObj& XmlObj::subobj(const std::string&key)
{
    if (!has(key)) {
        std::string err = "Did not have "+key;
        throw std::runtime_error(err);
    }
    XmlObj obj(*this, key);
    obj.set_path(_path, key);
    _gc.push_back(obj);
    return _gc[_gc.size()-1];
}

void XmlObj::getsubfields(std::vector<std::string>& fields)
{
    fields = _child_vects;
}

string XmlObj::attribute(const std::string&key)
{
    std::map<std::string, std::string>::const_iterator it = _attribute.find(key);
    if (it != _attribute.end()) {
        return it->second;
    } else {
        return "";
    }
}

}
