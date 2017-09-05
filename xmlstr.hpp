#ifndef __X_TO_STRUCT_XML_STR_HPP
#define __X_TO_STRUCT_XML_STR_HPP

#include <stdint.h>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <sstream>

#include "xstr.hpp"
//#include <iostream>
//using namespace std;

namespace x2struct {

class XmlStr:public XStr {
public:
    XmlStr(bool newline=false, int space=4);
    ~XmlStr();
public:
    void begin(const std::string&root, int space);
    void end(const std::string&root, int space);
    int  space();
    std::string toStr() const;
public:
    virtual void convert(const std::string&name, const std::string& data, int space, int index);

    void convert(const std::string&name, int16_t data, int space, int index);
    void convert(const std::string&name, uint16_t data, int space, int index);
    void convert(const std::string&name, int32_t data, int space, int index);
    void convert(const std::string&name, uint32_t data, int space, int index);
    void convert(const std::string&name, int64_t data, int space, int index);
    void convert(const std::string&name, uint64_t data, int space, int index);
    void convert(const std::string&name, float data, int space, int index);
    void convert(const std::string&name, double data, int space, int index);
    void convert(const std::string&name, bool data, int space, int index);

    template <typename TYPE>
    void convert(const std::string&name, const std::vector<TYPE> &data, int space, int index) {
        for (size_t i=0; i<data.size(); ++i) {
            this->convert(name, data[i], space, (int)i);
        }
    }

    template <typename TYPE>
    void convert(const std::string&name, const std::set<TYPE> &data, int space, int index) {
        int i;
        for (typename std::set<TYPE>::const_iterator it=data.begin(); it!=data.end(); ++it) {
            this->convert(name, *it, space, i++);
        }
    }

    template <typename TYPE>
    void convert(const std::string&name, const TYPE& data, int space, int index) {
        data.__struct_to_str(*this, name, space);
    }
private:
    template <typename TYPE>
    void base_convert(const std::string&name, const TYPE& data, int space) {
        if (_newline) {
            _ss<<std::string(space, ' ');
        }
        _ss<<'<'<<name<<'>'<<data<<"</"<<name<<'>';
        if (_newline) {
            _ss<<'\n';
        }
    }
private:
    bool _newline;
    int  _space;
    std::stringstream _ss;
};

}

#endif


