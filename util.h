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

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

namespace x2struct {

struct cmp_str
{
   bool operator()(char const *a, char const *b)
   {
      return strcmp(a, b) < 0;
   }
};


template <typename T>
std::string tostr(T data) {
    char buf[64];
    if (sizeof(data) <= 4) {
        sprintf(buf, "%d", (int)data);
    } else {
        sprintf(buf, "%ld", (int64_t)data);
    }
    return buf;
}

template <>
std::string tostr(double data);

template <>
std::string tostr(float data);

template <typename T>
T tonum(const std::string&str)
{
    T t;
    if (sizeof(t) <= 4) {
        int tmp;
        sscanf(str.c_str(), "%d", &tmp);
        t = (T)tmp;
    } else {
        int64_t tmp;
        sscanf(str.c_str(), "%ld", &tmp);
        t = (T)tmp;
    }
    return t;
}

template<>
float tonum<float>(const std::string&str);

template<>
double tonum<double>(const std::string&str);

size_t split(std::vector<std::string>&slice, const std::string&str, char c);
size_t split(std::vector<std::string>&slice, const std::string&str, const std::string& sp);

std::string alias_parse(const std::string&key, const std::string&alias, const std::string&type, bool *me);


}


