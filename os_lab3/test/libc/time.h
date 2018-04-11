#include "../include/defines.h"
using clock_t = long unsigned int;
#define CLOCKS_PER_SEC 25;
using time_t = long int;
struct tm {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_ymon;
    int tm_year;
    int tm_day;
    int tm_yday;
    int tm_isdst;
};
clock_t clock();
time_t time(time_t* tp);
double difftime(time_t time2, time_t time1);
time_t mktime(struct tm* tp);
char* asctime(const struct tm* tp);
char* ctime(const time_t tp);
struct tm* gmtime(const time_t tp);
struct tm* localtime(const time_t tp);
size_t strftime(char* s, size_t smax, const char* fmt, const struct tm* tp);

