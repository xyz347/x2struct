#include <time.h>

#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp> 

#include "xstr.hpp"

namespace x2struct {

using namespace ::std;

std::string XStr::fieldName(const std::string&oname, const std::string&aname)
{
    string nname;

    vector<string> types;
    boost::split(types, aname, boost::is_any_of(" "));
    for (size_t i=0; i<types.size(); ++i) {
        vector<string> tname;
        boost::split(tname, types[i], boost::is_any_of(":"));
        if (tname.size()==2 && tname[0]==_type) {
            nname = tname[1];
            break;
        } else if (tname.size()==1) {
            nname = types[i];
        }
    }

    if (!nname.empty()) {
        vector<string> tname;
        boost::split(tname, nname, boost::is_any_of(","));
        return tname[0];
    } else {
        return oname;
    }
}


}

