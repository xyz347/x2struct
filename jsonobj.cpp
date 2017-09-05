#include <stdexcept>
#include <fstream>
#include <json/json.h>
#include "jsonobj.hpp"
//#include <iostream>

namespace x2struct {

using namespace ::std;

JsonObj::JsonObj(const std::string& str, bool isfile):XObj("json"),__v(new Json::Value),_v(*__v)
{
    string err;
    Json::Reader reader;
    do {
        if (isfile) {
            std::ifstream fs(str, std::ifstream::binary);
            if (!fs) {
                err = "Open file["+str+"] fail.";
                break;
            } else if (!reader.parse(fs, _v) || !_v.isObject()) {
                err = "Parse json string["+str+"] fail.";
                break;
            }
        } else if (!reader.parse(str, _v) || !_v.isObject()) {
            err = "Parse json string["+str+"] fail.";
            break;
        }
        return;
    } while (false);

    throw std::runtime_error(err);
}

JsonObj::JsonObj(Json::Value& v):XObj("json"),__v(0),_v(v)
{
}

JsonObj::~JsonObj()
{
    if (0 != __v) {
        delete __v;
    }
}

void JsonObj::convert(std::string &val)
{
    val = _v.asString();
}

void JsonObj::convert(bool &val)
{
    val = _v.asBool();
}

void JsonObj::convert(int32_t &val)
{
    val = _v.asInt();
}

void JsonObj::convert(uint32_t &val)
{
    val = _v.asUInt();
}

void JsonObj::convert(int16_t &val)
{
    val = (int16_t)_v.asInt();
}

void JsonObj::convert(uint16_t &val)
{
    val = (uint16_t)_v.asUInt();
}

void JsonObj::convert(int64_t &val)
{
    val = _v.asInt64();
}

void JsonObj::convert(uint64_t &val)
{
    val = _v.asUInt64();
}

void JsonObj::convert(double &val)
{
    val = _v.asDouble();
}

void JsonObj::convert(float &val)
{
    val = _v.asFloat();
}

bool JsonObj::has(const std::string&key)
{
    return _v.isMember(key);
}

size_t JsonObj::size()
{
    return (size_t)_v.size();
}

XObj& JsonObj::operator[](const std::string&key)
{
    return subobj(key);
}

XObj& JsonObj::subobj(size_t index)
{
    if (_v.isArray()) {
        JsonObj obj(_v[(Json::ArrayIndex)index]);
        obj._key = _key;
        obj.set_path(_path, index);
        _gc.push_back(obj);
        return _gc[_gc.size()-1];
    } else if (index == 0) {
        return *this;
    } else {
        throw std::runtime_error("Out of index");
        return *((XObj*)0);
    }
}

XObj& JsonObj::subobj(const std::string&key)
{
    if (_v.isMember(key)) {
        JsonObj obj(_v[key]);
        obj._key = key;
        obj.set_path(_path, key);
        _gc.push_back(obj);
        return _gc[_gc.size()-1];
    } else {
        throw std::runtime_error("Did not have "+key);
        return *((XObj*)0);
    }
}

string JsonObj::attribute(const std::string&key)
{
    try {
        string val;
        subobj(key).convert(val);
        return val;
    } catch (...) {
        return "";
    }
}

void JsonObj::getsubfields(std::vector<std::string>& fields)
{
    fields = _v.getMemberNames();
}

}
