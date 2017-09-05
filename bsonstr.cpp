#include "bsonstr.hpp"

namespace x2struct {

using namespace std;


BsonStr::BsonStr(mongo::BSONArrayBuilder* parent_array):XStr("bson"),_parent_array(parent_array)
{
}

BsonStr::~BsonStr()
{
}

void BsonStr::begin(const string&root, int splen)
{
}

void BsonStr::end(const string&root, int splen)
{
}

int BsonStr::space()
{
    return 0;
}

string BsonStr::toStr()const 
{
    return "";
}


mongo::BSONObj BsonStr::toBson() 
{
    return _bobj.obj();
}

void BsonStr::convert(const string&name, int16_t data, int splen, int index)
{
    _bobj<<name<<(int)data;
}

void BsonStr::convert(const string&name, uint16_t data, int splen, int index)
{
    _bobj<<name<<(int)data;
}

void BsonStr::convert(const string&name, int32_t data, int splen, int index)
{
    _bobj<<name<<(int)data;
}

void BsonStr::convert(const string&name, uint32_t data, int splen, int index)
{
    _bobj<<name<<(int)data;
}

void BsonStr::convert(const std::string&name, int64_t data, int splen, int index)
{
    _bobj<<name<<(long long)data;
}

void BsonStr::convert(const std::string&name, uint64_t data, int splen, int index)
{
    _bobj<<name<<(long long)data;
}

void BsonStr::convert(const std::string&name, float data, int splen, int index)
{
    _bobj<<name<<(double)data;
}

void BsonStr::convert(const std::string&name, double data, int splen, int index)
{
    _bobj<<name<<data;
}

void BsonStr::convert(const std::string&name, bool data, int splen, int index)
{
    _bobj<<name<<data;
}

void BsonStr::convert(const string&name, const string& data, int splen, int index)
{
    _bobj<<name<<data;
}


// base type vector
#define BUILD_ARRAY(TYPE)                                       \
    mongo::BSONArrayBuilder array;                              \
    for (auto iter=data.begin(); iter!=data.end(); iter++) {    \
        array.append((TYPE)*iter);                              \
    }                                                           \
    do {                                                        \
        if (!name.empty()) {                                    \
            _bobj<<name<<array.arr();                           \
        } else if (_parent_array!=0) {                          \
            _parent_array->append(array.arr());                 \
        } else {                                                \
            /* something wrong*/                                \
        }                                                       \
    } while(false)

void BsonStr::convert(const std::string&name, const std::vector<int16_t>& data, int splen, int index)
{
    BUILD_ARRAY(int);
}

void BsonStr::convert(const std::string&name, const std::vector<uint16_t>& data, int splen, int index)
{
    BUILD_ARRAY(int);
}

void BsonStr::convert(const std::string&name, const std::vector<int32_t>& data, int splen, int index)
{
    BUILD_ARRAY(int);
}

void BsonStr::convert(const std::string&name, const std::vector<uint32_t>& data, int splen, int index)
{
    BUILD_ARRAY(int);
}

void BsonStr::convert(const std::string&name, const std::vector<int64_t>& data, int splen, int index)
{
    BUILD_ARRAY(long long);
}

void BsonStr::convert(const std::string&name, const std::vector<uint64_t>& data, int splen, int index)
{
    BUILD_ARRAY(long long);
}

void BsonStr::convert(const std::string&name, const std::vector<float>& data, int splen, int index)
{
    BUILD_ARRAY(double);
}

void BsonStr::convert(const std::string&name, const std::vector<double>& data, int splen, int index)
{
    BUILD_ARRAY(double);
}

void BsonStr::convert(const std::string&name, const std::vector<bool>& data, int splen, int index)
{
    BUILD_ARRAY(bool);
}

void BsonStr::convert(const std::string&name, const std::vector<std::string>& data, int splen, int index)
{
    BUILD_ARRAY(string);
}


// base type set
void BsonStr::convert(const std::string&name, const std::set<int16_t>& data, int splen, int index)
{
    BUILD_ARRAY(int);
}

void BsonStr::convert(const std::string&name, const std::set<uint16_t>& data, int splen, int index)
{
    BUILD_ARRAY(int);
}

void BsonStr::convert(const std::string&name, const std::set<int32_t>& data, int splen, int index)
{
    BUILD_ARRAY(int);
}

void BsonStr::convert(const std::string&name, const std::set<uint32_t>& data, int splen, int index)
{
    BUILD_ARRAY(int);
}

void BsonStr::convert(const std::string&name, const std::set<int64_t>& data, int splen, int index)
{
    BUILD_ARRAY(long long);
}

void BsonStr::convert(const std::string&name, const std::set<uint64_t>& data, int splen, int index)
{
    BUILD_ARRAY(long long);
}

void BsonStr::convert(const std::string&name, const std::set<float>& data, int splen, int index)
{
    BUILD_ARRAY(double);
}

void BsonStr::convert(const std::string&name, const std::set<double>& data, int splen, int index)
{
    BUILD_ARRAY(double);
}

void BsonStr::convert(const std::string&name, const std::set<bool>& data, int splen, int index)
{
    BUILD_ARRAY(bool);
}

void BsonStr::convert(const std::string&name, const std::set<std::string>& data, int splen, int index)
{
    BUILD_ARRAY(string);
}




}



