/*
* Copyright (C) 2019 YY Inc. All rights reserved.
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


#ifndef __X_WRITER_H
#define __X_WRITER_H


#include "xstdint.h"
#include <string>
#include <list>

#ifdef XTOSTRUCT_QT
#include <QString>
#include <QList>
#include <QMap>
#include <QVector>
#endif

namespace x2struct {

template<typename DOC>
class XWriter {
protected:
    typedef DOC doc_type;
    typedef XWriter<DOC> xdoc_type;
public:
    #ifdef XTOSTRUCT_QT
    doc_type& convert(const char*key, const QString &val) {
        std::string str = val.toStdString();
        doc_type *dt = (doc_type*)this;
        dt->convert(key, str);
        return *dt;
    }

    template<typename T>
    doc_type& convert(const char*key, const QList<T>&data) {
        std::list<T> sl = std::list<T>(data.begin(), data.end());
        doc_type *dt = (doc_type*)this;
        dt->convert(key, sl);
        return *dt;
    }

    template<typename T>
    doc_type& convert(const char*key, const QMap<std::string, T>&data) {
        std::map<std::string, T> sm = data.toStdMap();
        doc_type *dt = (doc_type*)this;
        dt->convert(key, sm);
        return *dt;
    }

    template<typename T>
    doc_type& convert(const char*key, const QMap<QString, T>&data) {
        std::map<std::string, T> sm;
        for (typename QMap<QString, T>::const_iterator iter=data.begin(); iter!=data.end(); iter++) {
            sm[iter.key().toStdString()] = iter.value();
        }
        doc_type *dt = (doc_type*)this;
        dt->convert(key, sm);
        return *dt;
    }

    template<typename T>
    doc_type& convert(const char*key, const QVector<T>&data) {
        std::vector<T> sv = data.toStdVector();
        doc_type *dt = (doc_type*)this;
        dt->convert(key, sv);
        return *dt;
    }
    #endif

    #ifdef XTOSTRUCT_SUPPORT_CHAR_ARRAY
    doc_type& convert(const char*key, const char val[]) {
        std::string str(val);
        return ((doc_type*)this)->convert(key, str);
    }
    #endif

    template <typename T>
    doc_type& convert(const char*key, const XType<T>& data) {
        doc_type* dt = (doc_type*)this;
        data.__struct_to_str(*dt, key);
        return *dt;
    }

    #ifdef __APPLE__
    doc_type& convert(const char*key, long val) {
        return ((doc_type*)this)->convert(key, (int64_t)val);
    }
    doc_type& convert(const char*key, unsigned long val) {
        return ((doc_type*)this)->convert(key, (uint64_t)val);
    }
    #endif
};

}

#endif
