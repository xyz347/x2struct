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

#include "util.hpp"
#include <iostream>

using namespace std;

namespace x2struct {

template <>
std::string tostr(double data)
{
    char buf[64];
    sprintf(buf, "%lf", data);
    return buf;
}

template <>
std::string tostr(float data)
{
    char buf[64];
    sprintf(buf, "%f", data);
    return buf;
}

template<>
float tonum<float>(const std::string&str)
{
    float t;
    sscanf(str.c_str(), "%f", &t);
    return t;
}

template<>
double tonum<double>(const std::string&str)
{
    double t;
    sscanf(str.c_str(), "%lf", &t);
    return t;
}

size_t split(std::vector<std::string>&slice, const std::string&str, char c)
{
    size_t last = 0;
    size_t pos = 0;
    while (string::npos != (pos=str.find(c, last))) {
        slice.push_back(str.substr(last, pos-last));
        last = pos+1;
    }

    slice.push_back(str.substr(last));
    return slice.size();
}

size_t split(std::vector<std::string>&slice, const std::string&str, const std::string&sp)
{
    size_t last = 0;
    size_t pos = 0;
    size_t len = sp.length();
    if (len == 0) {
        slice.push_back(str);
        return 1;
    }
    while (string::npos != (pos=str.find(sp, last))) {
        slice.push_back(str.substr(last, pos-last));
        last = pos+len;
    }

    slice.push_back(str.substr(last));
    return slice.size();
}

}


