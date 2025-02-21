#include "Types.h"

bool Tp_timeIn(const struct tm *tm1,const struct tm *begin, const struct tm *end) {
    if (
        tm1->tm_year >= begin->tm_year &&
        tm1->tm_mon >= begin->tm_mon &&
        tm1->tm_mday >= begin->tm_mday &&
        tm1->tm_hour >= begin->tm_hour &&
        tm1->tm_min >= begin->tm_min
        &&
        tm1->tm_year <= end->tm_year &&
        tm1->tm_mon <= end->tm_mon &&
        tm1->tm_mday <= end->tm_mday &&
        tm1->tm_hour <= end->tm_hour &&
        tm1->tm_min <= end->tm_min
    )
        return true;
    return false;
}

bool Tp_timeSameDay(const struct tm *tm1, const struct tm *tm2) {
    return ( 
        tm1->tm_year == tm2->tm_year &&
        tm1->tm_mon == tm2->tm_mon &&
        tm1->tm_mday == tm2->tm_mday
     );
}