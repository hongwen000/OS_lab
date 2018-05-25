#include "./time.h"
#include "./stdio.h"
#include "./sys/hhos.h"
#include "./string.h"

#ifdef _HHOS_LIBC_TEST
namespace hhlibc {

#endif

time_t time(time_t* pt)
{
    uint32_t t= system_call_get_timestamp();
    if(pt) *pt = t;
    return t;
}
static tm __LIBC__TM__;

static bool is_leap_year(uint16_t year)
{
    return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
}

static int weekday(int y, int m, int d){
    return (d += m < 3 ? y-- : y - 2, 23*m/9 + d + 4 + y/4- y/100 + y/400)%7;  
};

static int day_of_mon[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
struct tm* gmtime(const time_t tp)
{
    tm* ret = & __LIBC__TM__;
    ret->tm_year = 1970;
    ret->tm_mon = 1;
    ret->tm_mday = 1;
    int days = tp / 86400;
    while(days) 
    {
        int yday = 0;
        bool is_leap = is_leap_year(ret->tm_year);
        for(int i = 0; i < 12; ++i)
        {
            int day_this_mon = (i == 1 && is_leap)
                ? 29 : day_of_mon[i];
            if(days < day_this_mon)
            {
                ret->tm_mday = days + 1;
                ret->tm_yday = yday + 1;
                days = 0;
                break;
            }
            yday += day_this_mon;
            days -= day_this_mon;
            ret->tm_mon++;
        }
        if(!days) break;
        ret->tm_year++;
        ret->tm_mon = 1;
    }
    ret->tm_sec = tp % 60;
    ret->tm_min = (tp % 3600) / 60;
    ret->tm_hour = (tp / 3600) % 24;
    ret->tm_wday = weekday(ret->tm_year, ret->tm_mon, ret->tm_mday);
    return ret;
}


static char __LIBC__ASCTIME__[26];
char* asctime(const struct tm* tp)
{
    //char *wkname[7] = {"Mon","Tue","Wed","Thu", "Fri", "Sat", "Sun"};
    //char *monname[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    //memset(__LIBC__ASCTIME__, 0, 26);
    //memcpy(__LIBC__ASCTIME__, wkname[tp->tm_wday], 3);
    //__LIBC__ASCTIME__[3] = ' ';
    //memcpy(__LIBC__ASCTIME__ + 4, wkname[tp->tm_mon], 3);
    //__LIBC__ASCTIME__[7] = ' ';
    memset(__LIBC__ASCTIME__, 0, 26);
    sprintf(__LIBC__ASCTIME__,"%d/%d/%d-%d:%d:%d", tp->tm_year, tp->tm_mon, tp->tm_mday,
            tp->tm_hour, tp->tm_min, tp->tm_sec);
    return __LIBC__ASCTIME__;
}

#ifdef _HHOS_LIBC_TEST
}
#endif
