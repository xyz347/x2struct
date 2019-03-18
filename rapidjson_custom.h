#ifndef __X_RAPIDJSON_CUSTOME_H
#define __X_RAPIDJSON_CUSTOME_H

#include <cassert>
#define RAPIDJSON_NOEXCEPT_ASSERT(x) assert(x)

#include <stdexcept>
#define RAPIDJSON_ASSERT(x) if(!(x)) throw std::runtime_error(#x) 

#define RAPIDJSON_HAS_STDSTRING 1


#endif

