#include "sys_lib.h"
#include "../libc/stdio.h"
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
 
unsigned char sys_internal_second;
unsigned char sys_internal_minute;
unsigned char sys_internal_hour;
unsigned char sys_internal_day;
unsigned char sys_internal_month;
unsigned int sys_internal_year;

char sys_internal_time_str[20];
 
 
enum {
      cmos_address = 0x70,
      cmos_data    = 0x71
};
 
int get_update_in_progress_flag() {
      sys_outb(cmos_address, 0x0A);
      return (sys_inb(cmos_data) & 0x80);
}
 
unsigned char get_RTC_register(int reg) {
      sys_outb(cmos_address, reg);
      return sys_inb(cmos_data);
}
 
void read_rtc() {
      unsigned char last_second;
      unsigned char last_minute;
      unsigned char last_hour;
      unsigned char last_day;
      unsigned char last_month;
      unsigned char last_year;
      unsigned char registerB;
 
      while (get_update_in_progress_flag());
      sys_internal_second = get_RTC_register(0x00);
      sys_internal_minute = get_RTC_register(0x02);
      sys_internal_hour = get_RTC_register(0x04);
      sys_internal_day = get_RTC_register(0x07);
      sys_internal_month = get_RTC_register(0x08);
      sys_internal_year = get_RTC_register(0x09);
 
      do {
            last_second = sys_internal_second;
            last_minute = sys_internal_minute;
            last_hour = sys_internal_hour;
            last_day = sys_internal_day;
            last_month = sys_internal_month;
            last_year = sys_internal_year;
 
            while (get_update_in_progress_flag());           // Make sure an update isn't in progress
            sys_internal_second = get_RTC_register(0x00);
            sys_internal_minute = get_RTC_register(0x02);
            sys_internal_hour = get_RTC_register(0x04);
            sys_internal_day = get_RTC_register(0x07);
            sys_internal_month = get_RTC_register(0x08);
            sys_internal_year = get_RTC_register(0x09);
      } while( (last_second != sys_internal_second) || (last_minute != sys_internal_minute) || (last_hour != sys_internal_hour) ||
               (last_day != sys_internal_day) || (last_month != sys_internal_month) || (last_year != sys_internal_year));
 
      registerB = get_RTC_register(0x0B);
 
      if (!(registerB & 0x04)) {
            sys_internal_second = (sys_internal_second & 0x0F) + ((sys_internal_second / 16) * 10);
            sys_internal_minute = (sys_internal_minute & 0x0F) + ((sys_internal_minute / 16) * 10);
            sys_internal_hour = ( (sys_internal_hour & 0x0F) + (((sys_internal_hour & 0x70) / 16) * 10) ) | (sys_internal_hour & 0x80);
            sys_internal_day = (sys_internal_day & 0x0F) + ((sys_internal_day / 16) * 10);
            sys_internal_month = (sys_internal_month & 0x0F) + ((sys_internal_month / 16) * 10);
            sys_internal_year = (sys_internal_year & 0x0F) + ((sys_internal_year / 16) * 10);
      }
 
      if (!(registerB & 0x02) && (sys_internal_hour & 0x80)) {
            sys_internal_hour = ((sys_internal_hour & 0x7F) + 12) % 24;
      }
 
      sys_internal_year += (SYS_INTERNAL_CURRENT_YEAR / 100) * 100;
      if(sys_internal_year < SYS_INTERNAL_CURRENT_YEAR) sys_internal_year += 100;
      sprintf(sys_internal_time_str,"%d/%d/%d-%d:%d:%d\n", sys_internal_year, sys_internal_month, sys_internal_day, sys_internal_hour, sys_internal_minute, sys_internal_second);
}
