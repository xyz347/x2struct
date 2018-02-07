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

#define JSON_WRITER_BUFFER rapidjson::StringBuffer
#define JSON_WRITER_WRITER rapidjson::Writer<JSON_WRITER_BUFFER>
#define JSON_WRITER_PRETTY rapidjson::PrettyWriter<JSON_WRITER_BUFFER>

#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include "json_writer.h"

namespace x2struct {

using namespace ::std;

JsonWriter::JsonWriter(int indentCount, char indentChar)
{
    _buf = new JSON_WRITER_BUFFER;
    if (indentCount < 0) {
        _writer = new JSON_WRITER_WRITER(*_buf);
        _pretty = 0;
    } else {
        _pretty = new JSON_WRITER_PRETTY(*_buf);
        _pretty->SetIndent(indentChar, indentCount);
        _writer = 0;
    }
}

JsonWriter::~JsonWriter()
{
    if (0 != _buf) {
        delete _buf;
    }
    if (0 != _writer) {
        delete _writer;
    }
    if (0 != _pretty) {
        delete _pretty;
    }
}


JsonWriter& JsonWriter::convert(const char*key, const std::string& val)
{
    set_key(key);
    if (0 != _writer) {
        _writer->String(val);
    } else {
        _pretty->String(val);
    }
    return *this;
}

JsonWriter& JsonWriter::convert(const char*key, bool val)
{
    set_key(key);
    if (0 != _writer) {
        _writer->Bool(val);
    } else {
        _pretty->Bool(val);
    }
    return *this;
}

JsonWriter& JsonWriter::convert(const char*key, int16_t val)
{
    set_key(key);
    if (0 != _writer) {
        _writer->Int(val);
    } else {
        _pretty->Int(val);
    }
    return *this;
}

JsonWriter& JsonWriter::convert(const char*key, uint16_t val)
{
    set_key(key);
    if (0 != _writer) {
        _writer->Uint(val);
    } else {
        _pretty->Uint(val);
    }
    return *this;
}

JsonWriter& JsonWriter::convert(const char*key, int32_t val)
{
    set_key(key);
    if (0 != _writer) {
        _writer->Int(val);
    } else {
        _pretty->Int(val);
    }
    return *this;
}

JsonWriter& JsonWriter::convert(const char*key, uint32_t val)
{
    set_key(key);
    if (0 != _writer) {
        _writer->Uint(val);
    } else {
        _pretty->Uint(val);
    }
    return *this;
}

JsonWriter& JsonWriter::convert(const char*key, int64_t val)
{
    set_key(key);
    if (0 != _writer) {
        _writer->Int64(val);
    } else {
        _pretty->Int64(val);
    }
    return *this;
}

JsonWriter& JsonWriter::convert(const char*key, uint64_t val)
{
    set_key(key);
    if (0 != _writer) {
        _writer->Uint64(val);
    } else {
        _pretty->Uint64(val);
    }
    return *this;
}

JsonWriter& JsonWriter::convert(const char*key, double val)
{
    set_key(key);
    if (0 != _writer) {
        _writer->Double(val);
    } else {
        _pretty->Double(val);
    }
    return *this;
}

JsonWriter& JsonWriter::convert(const char*key, float val)
{
    set_key(key);
    if (0 != _writer) {
        _writer->Double(val);
    } else {
        _pretty->Double(val);
    }
    return *this;
}

std::string JsonWriter::toStr()
{
    return _buf->GetString();
}

void JsonWriter::set_key(const char*key)
{
    if (0!=key && key[0]!='\0') {
        if (0 != _writer) {
            _writer->Key(key);
        } else {
            _pretty->Key(key);
        }
    }
}
void JsonWriter::array_begin()
{
    if (0 != _writer) {
        _writer->StartArray();
    } else {
        _pretty->StartArray();
    }
}
void JsonWriter::array_end()
{
    if (0 != _writer) {
        _writer->EndArray();
    } else {
        _pretty->EndArray();
    }
}
void JsonWriter::object_begin()
{
    if (0 != _writer) {
        _writer->StartObject();
    } else {
        _pretty->StartObject();
    }
}
void JsonWriter::object_end()
{
    if (0 != _writer) {
        _writer->EndObject();
    } else {
        _pretty->EndObject();
    }
}

const std::string& JsonWriter::type()
{
    static string t("json");
    return t;
}

}
