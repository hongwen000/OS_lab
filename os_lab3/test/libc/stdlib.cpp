#include "./stdlib.h"
#include "./string.h"
#include "./ctype.h"

#ifdef _HHOS_LIBC_TEST
namespace hhlibc {

#else

void abort()
{
    asm volatile ("int $0x20\n\t");
}
#endif
int abs(int n)
{
    return n > 0 ? n : -n;
}

long labs( long n )
{
    return n > 0 ? n : -n;
}

div_t div( int x, int y )
{
    if(x == 0) return div_t{0, 0};
    div_t r = div(x/2, y);
    r.quot = 2 * r.quot;
    r.rem = 2 * r.rem;
    if(x % 2) r.rem = r.rem + 1;
    if(r.rem >= y) 
    {
        r.quot = r.quot + 1;
        r.rem = r.rem - y;
    }
    return r;
}

ldiv_t ldiv( long x, long y )
{
    if(x == 0) return ldiv_t{0, 0};
    ldiv_t r = ldiv(x/2, y);
    r.quot = 2 * r.quot;
    r.rem = 2 * r.rem;
    if(x % 2) r.rem = r.rem + 1;
    if(r.rem >= y) 
    {
        r.quot = r.quot + 1;
        r.rem = r.rem - y;
    }
    return r;
}
int atoi(const char* s)
{
    return (int)strtol(s, (char**)nullptr, 10);
}

long atol(const char* s)
{
    return strtol(s, (char**)nullptr, 10);
}
//TODO:
//与标准的差别：
//只实现了16位及以下的转换
//未做溢出检测，溢出情况下不会有错误指示
long strtol( const char *str, char **str_end, int base )
{
    auto start = str;   
    //start指向第一个非白空格位置
    while(isspace(*start))
        ++start;
    size_t len_num = strlen(start);//非白空格数字串长度
    if(len_num == 0)
    {
        
        if(str_end) *str_end = const_cast<char*>(str);
        return 0;
    }
    auto high = start;   //指向真正的数字高位在字符串中的起始位置
    bool auto_detected = false;
    int sign = 1;
    size_t sign_char_bit = 0;
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
    auto char_to_digit = [](auto ch){
        int digit;
        if(ch >= '0' && ch <= '9')
            digit = ch - '0';
        else if(ch >='A' && ch <= 'F')
            digit = ch - 'A' + 10;
        else if(ch >='a' && ch <= 'f')
            digit = ch - 'a' + 10;
        else
            digit = -1;
        return digit;
    };
    bool has_vaild_char = false;
    auto low = start;   
    //使low指向数字低位后的一个位置
    while(!isspace(*low) &&             //判断是否是白空格
          (*low != '\0') &&             //判断是否字符串结束
          char_to_digit(*low) != -1 &&  //判断是否是合法数字
          char_to_digit(*low) < base    //判断是否是该进制
          )
    {
        ++low;
        has_vaild_char = true;
    }
    if(!has_vaild_char)
    {
        //未发现可转换字符
        if (str_end) *str_end = const_cast<char*>(str);
        return 0;
    }
    long power = 1;
    long ret = 0;
    if(str_end) *str_end = const_cast<char*>(low);
    for(auto p = low - 1; p >= high; --p)
    {
        int digit = char_to_digit(*p);
        ret += power * digit;
        
        power *= base;
    }
    ret *= sign;
    return ret;
}

#ifdef _HHOS_LIBC_TEST
}
#endif
