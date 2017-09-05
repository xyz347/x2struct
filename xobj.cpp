#include <time.h>

#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp> 

#include "x2struct.hpp"
#include "xobj.hpp"

namespace x2struct {

using namespace ::std;

std::string XObj::hasa(const std::string&oname, const std::string&aname, bool &me)
{
    string allname;
    string strictname;

    vector<string> types;
    boost::split(types, aname, boost::is_any_of(" "));
    for (size_t i=0; i<types.size(); ++i) {
        vector<string> tname;
        boost::split(tname, types[i], boost::is_any_of(":"));
        if (tname.size()==2 && tname[0]==_type) {
            strictname = tname[1];
            break;
        } else if (tname.size()==1) {
            allname = types[i];
        }
    }

    if (!strictname.empty()) {       // typename first
        vector<string> opts;
        boost::split(opts, strictname, boost::is_any_of(","));
        if (has(opts[0])) {
            for (size_t i=1; i<opts.size(); ++i) {
                if (opts[i]==X2STRUCT_OPT_ME) {
                    me = true;
                    break;
                }
            }
            return opts[0];
        }
    }

    if (!allname.empty()) {         // then allname 
        vector<string> opts;
        boost::split(opts, allname, boost::is_any_of(","));
        if (has(opts[0])) {
            for (size_t i=1; i<opts.size(); ++i) {
                if (opts[i]==X2STRUCT_OPT_ME) {
                    me = true;
                    break;
                }
            }
            return opts[0];
        }
    }

    if (has(oname)) {               // original last
        return oname;
    } else {
        return "";
    }
}

void XObj::set_path(const std::string&father, const std::string&key)
{
    if (!father.empty()) {
        _path.append(father);
        _path.append(".");
    }
    _path.append(key);
}

void XObj::set_path(const std::string&father, size_t index)
{
    _path.append(father);
    _path.append("[");
    _path.append(boost::lexical_cast<std::string>(index));
    _path.append("]");
}

void XObj::me_exception(const std::string&key)
{
    std::string err="MUST EXIST node [";
    if (!_path.empty()) {
        err.append(_path);
        err.append(".");
    }
    err.append(key);
    err.append("] not exist");
    throw std::runtime_error(err);                                  \
}


}

