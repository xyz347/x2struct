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


#pragma once

#include <string>
#include "util.hpp"


namespace x2struct {

class XStr {
public:
    XStr(const std::string&type):_type(type){}
    virtual ~XStr(){}
public:
    virtual std::string fieldName(const std::string&oname, const std::string&aname); // alias name
public:
    virtual void convert(const std::string&name, const std::string& data, int space, int index) = 0;
private:
    std::string _type;  // xml/json/bson/config
};

}

