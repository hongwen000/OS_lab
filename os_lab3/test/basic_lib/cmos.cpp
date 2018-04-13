#include "sys_lib.h"
#include "../libc/stdio.h"
#include "../libc/string.h"
#include "../libc/stdlib.h"
void read_cmos (unsigned char array [])
{
   unsigned char tvalue, index;
 
   for(index = 0; index < 128; index++)
   {
       asm volatile(
            "cli\n\t"
            "movb %1, %%al\n\t"
            "out %%al, $0x70\n\t"
            "in $0x71, %%al\n\t"
            "sti\n\t"
            "movb %%al, %0\n\t"
            :"=r"(tvalue)
            :"r"(index)
            :"%eax"
               );
       array[index] = tvalue;
   }
}

#define SYS_INTERNAL_CURRENT_YEAR 2018
 
static uint8_t sys_sec;
static uint8_t sys_minute;
static uint8_t sys_hour;
static uint8_t sys_day;
static uint8_t sys_month;
static uint16_t sys_year;

char sys_internal_time_str[20];
 
 
#define CMOS_SELECT 0x70
#define CMOS_DATA 0x71
 
int is_rdt_updating() {
      sys_outb(CMOS_SELECT, 0x0A);
      return (sys_inb(CMOS_DATA) & 0x80);
}
 
uint8_t read_rtc(int reg) {
      sys_outb(CMOS_SELECT, reg);
      return sys_inb(CMOS_DATA);
}
 
void read_rtc() {
      uint8_t _sec;
      uint8_t _min;
      uint8_t _hour;
      uint8_t _day;
      uint8_t _month;
      uint8_t _year;
      uint8_t reg_B;
 
      while (is_rdt_updating());
      sys_sec = read_rtc(0x00);
      sys_minute = read_rtc(0x02);
      sys_hour = read_rtc(0x04);
      sys_day = read_rtc(0x07);
      sys_month = read_rtc(0x08);
      sys_year = read_rtc(0x09);
      _sec = sys_sec;
      _min = sys_minute;
      _hour = sys_hour;
      _day = sys_day;
      _month = sys_month;
      _year = sys_year;
      while( (_sec != sys_sec) || (_min != sys_minute) || (_hour != sys_hour) ||
               (_day != sys_day) || (_month != sys_month) || (_year != sys_year))
      {
            _sec = sys_sec;
            _min = sys_minute;
            _hour = sys_hour;
            _day = sys_day;
            _month = sys_month;
            _year = sys_year;
            while (is_rdt_updating());
            sys_sec = read_rtc(0x00);
            sys_minute = read_rtc(0x02);
            sys_hour = read_rtc(0x04);
            sys_day = read_rtc(0x07);
            sys_month = read_rtc(0x08);
            sys_year = read_rtc(0x09);
      }
      reg_B = read_rtc(0x0B);
      // Unpack BCD
      if (!(reg_B & 0x04)) {
            sys_sec = (sys_sec & 0x0F) + ((sys_sec / 16) * 10);
            sys_minute = (sys_minute & 0x0F) + ((sys_minute / 16) * 10);
            sys_hour = ( (sys_hour & 0x0F) + (((sys_hour & 0x70) / 16) * 10) ) | (sys_hour & 0x80);
            sys_day = (sys_day & 0x0F) + ((sys_day / 16) * 10);
            sys_month = (sys_month & 0x0F) + ((sys_month / 16) * 10);
            sys_year = (sys_year & 0x0F) + ((sys_year / 16) * 10);
      }
      if (!(reg_B & 0x02) && (sys_hour & 0x80)) {
            sys_hour = ((sys_hour & 0x7F) + 12) % 24;
      }
 
      sys_year += (SYS_INTERNAL_CURRENT_YEAR / 100) * 100;
      if(sys_year < SYS_INTERNAL_CURRENT_YEAR) sys_year += 100;
      memset(sys_internal_time_str, 0, 20);
      sprintf(sys_internal_time_str,"%d %d %d %d %d %d", sys_year, sys_month, sys_day, sys_hour, sys_minute, sys_sec);
}

static bool is_leap_year(uint16_t year)
{
    return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
}

extern "C" uint32_t sys_get_timestamp()
{
    read_rtc();
    int sys_sec = 0;
    int sys_minute = 0;
    int sys_hour = 0;
    int sys_day = 0;
    int sys_month = 0;
    int sys_year;
    sscanf(sys_internal_time_str,"%d %d %d %d %d %d", 
            &sys_year, &sys_month, &sys_day,
            &sys_hour, &sys_minute, &sys_sec);
    uint32_t ts = 0;
    uint8_t cnt_non_leap = 0;
    uint8_t cnt_leap = 0;
    for( uint16_t i = 1970; i < sys_year; i++ )
    {
        is_leap_year(i) ? cnt_leap++ : cnt_non_leap++;
    }
    ts += ((cnt_non_leap*365) + (cnt_leap*366)) * 86400;
    int day_of_mon[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    for(uint8_t i = 0; i < (sys_month - 1); i++ )
    { 
        ts += (i == 1 && is_leap_year(sys_year)) ? 
            (29 * 86400) : (day_of_mon[i] * 86400);
    }
    ts += (sys_day-1) * 86400; 
    ts += sys_hour * 3600;
    ts += sys_minute * 60;
    ts += sys_sec;
    return ts;
}
