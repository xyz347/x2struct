#include <stdexcept>
#include <libconfig.h++>
#include "configobj.hpp"
#include <sstream>
#include <iostream>

namespace x2struct {

using namespace ::std;
using namespace ::libconfig;

ConfigObj::ConfigObj(const std::string& str, bool isfile, const std::string&root):XObj("config"),_c(new libconfig::Config),_s(0)
{
    stringstream err;
    try {
        if (isfile) {
            _c->readFile(str.c_str());
        } else {
            _c->readString(str);
        }
        _s = &_c->getRoot();
        if (root.empty()) {
            _s = &((*_s)[0]);
        } else {
            _s = &((*_s)[root]);
        }
        return;
    } catch (const FileIOException &e) {
        err<<"load file["<<str<<"] failed:"<<e.what();
    } catch (const ParseException &e) {
        err<<"parse ["<<str<<"] error at ["<<e.getFile()<<":"<<e.getLine()<<" errinfo:"<<e.getError();
    } catch (...) {
        err<<"Unknow exception when load["<<str<<"]";
    }

    throw std::runtime_error(err.str());
}

ConfigObj::ConfigObj(libconfig::Setting& s):XObj("config"),_c(0),_s(&s)
{
}

ConfigObj::~ConfigObj()
{
    if (0 != _c) {
        delete _c;
    }
}

void ConfigObj::convert(std::string &val)
{
    val = _s->c_str();
}

void ConfigObj::convert(bool &val)
{
    val = *_s;
}

void ConfigObj::convert(int32_t &val)
{
    val = *_s;
}

void ConfigObj::convert(uint32_t &val)
{
    val = *_s;
}

void ConfigObj::convert(int16_t &val)
{
    val = (int16_t)(int)(*_s);
}

void ConfigObj::convert(uint16_t &val)
{
    val = (uint16_t)(unsigned int)(*_s);
}

void ConfigObj::convert(int64_t &val)
{
    val = *_s;
}

void ConfigObj::convert(uint64_t &val)
{
    val = *_s;
}

void ConfigObj::convert(double &val)
{
    val = *_s;
}

void ConfigObj::convert(float &val)
{
    val = *_s;
}

bool ConfigObj::has(const std::string&key)
{
    return _s->exists(key.c_str());
}

size_t ConfigObj::size()
{
    return (size_t)_s->getLength();
}

XObj& ConfigObj::operator[](const std::string&key)
{
    return subobj(key);
}

XObj& ConfigObj::subobj(size_t index)
{
    if (_s->isList()) {
        ConfigObj obj((*_s)[index]);
        obj._key = _key;
        obj.set_path(_path, index);
        _gc.push_back(obj);
        return _gc[_gc.size()-1];
    } else if (index == 0) {
        return *this;
    } else {
        throw std::runtime_error(_key+" Out of index");
        return *((XObj*)0);
    }
}

XObj& ConfigObj::subobj(const std::string&key)
{
    if (_s->exists(key)) {
        ConfigObj obj((*_s)[key.c_str()]);
        obj._key = key;
        obj.set_path(_path, key);
        _gc.push_back(obj);
        return _gc[_gc.size()-1];
    } else {
        throw std::runtime_error("Did not have "+key);
        return *((XObj*)0);
    }
}

string ConfigObj::attribute(const std::string&key)
{
    try {
        string val;
        subobj(key).convert(val);
        return val;
    } catch (...) {
        return "";
    }
}

void ConfigObj::exception(const std::exception& e)
{
    stringstream err;
    if (0 != dynamic_cast<const libconfig::FileIOException*>(&e)) {
        err<<"load file failed: "<<e.what();
    } else if (0 != dynamic_cast<const libconfig::ParseException*>(&e)) {
        const libconfig::ParseException*ne = dynamic_cast<const libconfig::ParseException*>(&e);
        err<<"parse config fail at "<<ne->getFile()<<":"<<ne->getLine()<<". error["<<ne->getError()<<"]";
    } else if (0 != dynamic_cast<const libconfig::SettingNotFoundException*>(&e)) {
        const libconfig::SettingNotFoundException*ne = dynamic_cast<const libconfig::SettingNotFoundException*>(&e);
        err<<"SettingNotFoundException: "<<ne->getPath();
    } else if (0 != dynamic_cast<const libconfig::SettingTypeException*>(&e)) {
        const libconfig::SettingTypeException*ne = dynamic_cast<const libconfig::SettingTypeException*>(&e);
        err<<"SettingTypeException: "<<ne->getPath();
    } else {
        err<<"Unknow exception:"<<e.what();
    }
    throw std::runtime_error(err.str());
}

}

