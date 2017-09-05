#include "xmlstr.hpp"

namespace x2struct {

using namespace std;


XmlStr::XmlStr(bool newline, int space)
    :XStr("xml"), _newline(newline),_space(space)
{
}

XmlStr::~XmlStr()
{
}

void XmlStr::begin(const string&root, int space)
{
    if (_newline) {
        _ss<<string(space, ' ')<<'<'<<root<<">\n";
    } else {
        _ss<<'<'<<root<<'>';
    }
}

void XmlStr::end(const string&root, int space)
{
    if (_newline) {
        _ss<<string(space, ' ')<<"</"<<root<<">\n";
    } else {
        _ss<<"</"<<root<<'>';
    }
}

int  XmlStr::space()
{
    return _newline?_space:0;
}

string XmlStr::toStr()const 
{
    return _ss.str();
}

void XmlStr::convert(const string&name, int16_t data, int space, int index)
{
    base_convert(name, data, space);
}

void XmlStr::convert(const string&name, uint16_t data, int space, int index)
{
    base_convert(name, data, space);
}

void XmlStr::convert(const string&name, int32_t data, int space, int index)
{
    base_convert(name, data, space);
}

void XmlStr::convert(const string&name, uint32_t data, int space, int index)
{
    base_convert(name, data, space);
}

void XmlStr::convert(const std::string&name, int64_t data, int space, int index)
{
    base_convert(name, data, space);
}

void XmlStr::convert(const std::string&name, uint64_t data, int space, int index)
{
    base_convert(name, data, space);
}

void XmlStr::convert(const std::string&name, float data, int space, int index)
{
    base_convert(name, data, space);
}

void XmlStr::convert(const std::string&name, double data, int space, int index)
{
    base_convert(name, data, space);
}

void XmlStr::convert(const std::string&name, bool data, int space, int index)
{
    base_convert(name, data?"true":"false", space);
}

void XmlStr::convert(const string&name, const string& data, int space, int index)
{
    base_convert(name, data, space);
}

}



