#ifndef __X_TO_STRUCT_BSON_HPP
#define __X_TO_STRUCT_BSON_HPP


#include "x2struct.hpp"
#include "bsonstr.hpp"
#include "bsonobj.hpp"

/*
   bson depend on bson library in header. so special it
*/
namespace x2struct {

class B {
public:
    template <typename TYPE>
    static bool loadbson(const mongo::BSONObj&bobj, TYPE&t) {
        BsonObj obj(bobj);
        t.__x_to_struct(obj);
        return true;
    }
    template <typename TYPE>
    static mongo::BSONObj tobson(const TYPE& t) {
        BsonStr obj;
        t.__struct_to_str(obj,"",0);
        return obj.toBson();
    }
};

}

#endif

