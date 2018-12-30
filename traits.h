/*
* Copyright (C) 2019 YY Inc. All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License"); 
* you may not use this file except in compliance with the License. 
* You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, 
* software distributed under the License is distributed on an "AS IS" BASIS, 
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
* See the License for the specific language governing permissions and 
* limitations under the License.
*/


#ifndef __X_TO_TRAITS_H
#define __X_TO_TRAITS_H

#if __GXX_EXPERIMENTAL_CXX0X__ || _MSC_VER>=1700
#include <type_traits>
#endif

using namespace std;

#if __GXX_EXPERIMENTAL_CXX0X__ || _MSC_VER>=1700
//template <class T>
//using x_for_class = typename std::enable_if<std::is_class<T>::value, int>::type;

//template <class T>
//using x_for_enum = typename std::enable_if<std::is_enum<T>::value, int>::type;

// <c++11 not support using template. so we use macro all
#define x_for_class(T) typename std::enable_if<std::is_class<T>::value, int>::type
#define x_for_enum(T)  typename std::enable_if<std::is_enum<T>::value, int>::type
#else
template<bool B, class T = void>
struct x_enable_if {};

template <class T>
struct x_enable_if<true, T> { typedef T type; };


template <class T>
struct x_tovoid {typedef void type;};

template <class T, typename=void>
struct x_is_class{static bool const value = false;};

template <class T>
struct x_is_class<T, typename x_tovoid<int T::*>::type >{static bool const value = true;};


#define x_for_class(T) typename x_enable_if<x_is_class<T>::value, int>::type
// too complicated to implement is_enum
#define x_for_enum(T)  typename x_enable_if<!x_is_class<T>::value, int>::type

#endif

#endif

