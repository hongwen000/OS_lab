#include "stdlib.h"
#include "string.h"
#include "ctype.h"

#ifdef _HHOS_LIBC_TEST
namespace hhlibc {

#endif

long strtol( const char *str, char **str_end, int base )
{
    auto start = str;   //指向第一个非白空格位置
    while(isspace(*start))
        ++start;
    size_t len_num = strlen(start);//非白空格数字串长度
    if(len_num == 0)
    {
        if(str_end) *str_end = NULL;
        return 0;
    }
    auto low = start;   //指向数字低位后的一个位置
    while(!isspace(*low) && (*low != '\0'))
        ++low;
    auto high = start;   //指向真正的数字高位在字符串中的起始位置
    bool auto_detected = false;
    int sign = 1;
    int sign_char_bit = 0;
    if(*high == '+' || *high == '-')
    {
        sign = (*high == '+') ? 1 : -1;
        ++high;
        sign_char_bit = 1;
    }
    if(base == 16 || (base == 0 && !auto_detected))
    {
        //如果存在十六进制前缀0x
        if(len_num > 1 + sign_char_bit && 
           *high == '0' && 
           (*(high + 1) == 'x' || *(high + 1) == 'X'))
           {
             high += 2;
             base = 16;
             auto_detected = true;
           }
    }
    if(base == 8 || (base == 0 && !auto_detected))
    {
        if(len_num > sign_char_bit && *high == 0)   //存在八进制前缀0
        {
            high += 1;
            base = 8;
            auto_detected = true;
        }
    }
    //未发现前缀，默认10进制
    if(base == 0 && !auto_detected)
    {
        base = 10;
        auto_detected = true;
    }
    int power = 1;
    long ret = 0;
    if(str_end) *str_end = const_cast<char*>(low);
    for(auto p = low - 1; p >= high; --p)
    {
        char ch = *p;
        int digit = 0;
        if(ch >= '0' && ch <= '9')
            digit = ch - '0';
        else if(ch >='A' && ch <= 'F')
            digit = ch - 'A' + 10;
        else if(ch >='a' && ch <= 'f')
            digit = ch - 'a' + 10;
        else
        {
            ret = 0; //在任意进制下都不合法的字符，重置转换结果
            power = 1;
            if(str_end) *str_end = const_cast<char*>(low);
        }
        if(digit >= base)   //在该进制下不合法的字符，重置
        {
            ret = 0;
            power = 1;
            if(str_end) *str_end = const_cast<char*>(low);
        }
        ret += power * digit;
        power *= base;
    }
    return ret;
}

#ifdef _HHOS_LIBC_TEST
}
#endif