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

#include <map>
#include <vector>
#include <string.h>

#include "xreader.h"

#ifndef XML_READER_DOCUMENT
#define XML_READER_DOCUMENT void
#define XML_READER_NODE     void
#endif

namespace x2struct {


// xml没有数组，适配起来有点恶心
class XmlReader:public XReader<XmlReader> {
public:
    using xdoc_type::convert;
    XmlReader(const std::string& str, bool isfile=false);
    ~XmlReader();
public: // convert
    void convert(std::string &val);
    void convert(bool &val);
    void convert(int16_t &val);
    void convert(uint16_t &val);
    void convert(int32_t &val);
    void convert(uint32_t &val);
    void convert(int64_t &val);
    void convert(uint64_t &val);
    void convert(double &val);
    void convert(float &val);

    const std::string& type();
    bool has(const char*key);
    size_t size(bool to_vec=true);
    XmlReader operator[](const char *key);
    XmlReader operator[](size_t index);
    XmlReader begin();
    XmlReader next();
    operator bool() const;

    std::string attribute(const char* key); // overwite

private:
    typedef std::vector<XML_READER_NODE*> sibling_type;
    typedef std::vector<sibling_type> child_type;
    typedef std::map<const char*, size_t, cmp_str> child_index_type;
    

    XmlReader(const sibling_type* val, const XmlReader*parent, const char*key);
    XmlReader(const sibling_type* val, const XmlReader*parent, size_t index);
    XmlReader(const XML_READER_NODE* val, const XmlReader*parent, size_t index);
    void init();

    XML_READER_DOCUMENT* _doc;
    char *_xml_data;

    const XML_READER_NODE* _val;
    const sibling_type *_siblings;
    child_type _childs;
    child_index_type _child_index; // 记录某个key在_childs中的索引
    mutable child_index_type::iterator _iter;
};

}


