/*    Copyright 2014 MongoDB Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#pragma once

// Define to 1 if SSL support is enabled
#define MONGO_SSL 1

// Define to 1 if SASL support is enabled
// #undef MONGO_SASL

// Define to 1 if unistd.h is available
#define MONGO_HAVE_HEADER_UNISTD_H 1

// Define to 1 if C++11 <atomic> is available
#define MONGO_HAVE_CXX11_ATOMICS 1

// Define to 1 if GCC style __atomic functions are available
// #undef MONGO_HAVE_GCC_ATOMIC_BUILTINS

// Define to 1 if GCC style __sync functions are available
// #undef MONGO_HAVE_GCC_SYNC_BUILTINS

// Define to target byte order (1234 vs 4321)
#define MONGO_BYTE_ORDER 1234

// Define to 1 if std::is_trivially_copyable is supported
// #undef MONGO_HAVE_STD_IS_TRIVIALLY_COPYABLE

// Define to 1 if this platform provides the strnlen(3) function
// #undef MONGO_HAVE_STRNLEN

