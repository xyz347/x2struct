/*
* Copyright (C) 2017 YY Inc. All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License"); 
* you may not use this file except in compliance with the License. 
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, 
* software distributed under the License is distributed on an "AS IS" BASIS, 
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
* See the License for the specific language governing permissions and 
* limitations under the License.
*/


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

