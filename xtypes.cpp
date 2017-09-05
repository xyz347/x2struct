#include <time.h>
#include <boost/date_time/posix_time/posix_time.hpp> 

#include "xtypes.hpp"

namespace x2struct {

std::string XDate::to_string() const
{
    time_t tt = (time_t)unix_time;
    tm     ttm;

    localtime_r(&tt, &ttm);
    boost::posix_time::ptime pt = boost::posix_time::ptime_from_tm(ttm);
    std::string str = boost::posix_time::to_iso_extended_string(pt);
    str[10] = ' ';
    return str;
}

void XDate::from_string(const std::string&str)
{
    time_t tt = 0;
    boost::posix_time::ptime pt = boost::posix_time::time_from_string(str);    // no try here. if exception, just throw it.
    tm tm1 = boost::posix_time::to_tm(pt);
    tt = mktime(&tm1);
    unix_time = (int64_t)tt;
}

}


