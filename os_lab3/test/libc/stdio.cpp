#include "./stdio.h"
#include "./string.h"
#include "./stdlib.h"
#include "./ctype.h"

#ifdef _HHOS_LIBC_TEST
namespace hhlibc {

#endif
#ifndef _HHOS_LIBC_TEST
int putchar( int ch )
{
    //sys_get_current_tty()->putchar(ch);
    system_call_putchar(ch);
    //sys_tty_putchar(ch);
    return ch;
}

int puts(const char* string)
{
    auto len = strlen(string);
    for(size_t i = 0; i < len; ++i)
    {
        putchar(*(string + i));
    }
    return 0;
}

#endif

int vsprintf( char* buffer, const char* format, va_list vlist )
{
    auto len_fmt = strlen(format);
    auto p_fmt = format; 
    auto p_buf = buffer;
    while(len_fmt--)
    {
        if(*p_fmt != '%')
        {
            *(p_buf++) = *(p_fmt++);    //非控制字符
            continue;
        }
        else
        {
            //此时p_fmt = '%'
            char specifier = *(++p_fmt); //%后面一个字符是specifier
            ++p_fmt;                     //记录specifier并跳过
            switch (specifier)
            {
                case 'c':
                {
                    /*
                    依据C语言标准
                    If you pass an integral value with conversion rank 
                    smaller than that of int (e. g. char, bool or short)
                     to a function taking a variable number of arguments, 
                     it will be converted to int
                    */
                    *(p_buf++) = va_arg(vlist, int); 
                    break;
                }
                case 's':
                {
                    const char * str = va_arg(vlist, char*);
                    strcpy(p_buf, str);
                    p_buf += strlen(str);               //p应当永远指向有意义的字符后面一位
                    break;
                }
                case 'd':
                case 'i':
                {
                    int num = va_arg(vlist, int);
                    itoa(p_buf, num, 10);
                    p_buf += strlen(p_buf);             //同理
                    break;
                }
                case 'o':
                {
                    unsigned int num = va_arg(vlist, int);
                    itoa(p_buf, num, 8);
                    p_buf += strlen(p_buf);             //同理
                    break;
                }                
                case 'x':
                {
                    unsigned int num = va_arg(vlist, int);
                    itoa(p_buf, num, 16);
                    p_buf += strlen(p_buf);             //同理
                    break;
                }                
                case 'X':
                {
                    unsigned int num = va_arg(vlist, int);
                    itoa(p_buf, num, 16, true);
                    p_buf += strlen(p_buf);             //同理
                    break;
                }                
                case 'u':
                {
                    unsigned int num = va_arg(vlist, int);
                    itoa(p_buf, num, 10);
                    p_buf += strlen(p_buf);             //同理
                    break;
                }                
                default:
                {
                    break;
                }
            }
        }
    }
    *p_buf = '\0';
    return p_buf - buffer;
}

int sprintf( char *buffer, const char *format, ... )
{
	int ret;
	va_list va;
	va_start(va, format);
	ret = vsprintf(buffer, format, va);
	va_end(va);
    return ret;
}

#ifndef _HHOS_LIBC_TEST
static char printbuf[80*25];
//TODO 关于这个返回值
int printf( const char* format, ... )
{
	int ret;
	va_list va;
	va_start(va, format);
	ret = vsprintf(printbuf, format, va);
	va_end(va);
    puts(printbuf);
    return ret;
}
#endif


#ifndef _HHOS_LIBC_TEST
int getchar(void)
{
    int ch = system_call_getchar();
    return ch;
}
char *gets( char *str )
{
    int cnt = 0;
    char ch;
    while(true)
    {
        ch = getchar();
        if (ch == '\b')
        {
            if (cnt > 0)
                putchar(ch);
            continue;
        }
        putchar(ch);
        if(ch == 13 || ch == 10) {
            putchar('\n');
            break;
        }
        *(str++) = ch;
    }
    *str = '\0';
    return str;
}
#endif

int vsscanf( const char* buffer, const char* format, va_list vlist )
{
    auto len_fmt = strlen(format);
    auto p_fmt = format; 
    auto p_buf = buffer;
    bool error = false;
    int ret = 0;
    while(static_cast<size_t>(p_fmt - format) < len_fmt)
    {
        if(error) break;
        if(isspace(*p_fmt)) //略过白空格
        {
            p_fmt++;
            while(isspace(*buffer)) buffer++;
            continue;
        }
        else if(*p_fmt == '%') //格式控制符
        {
            p_fmt++;
            char specifier = *p_fmt;
            p_fmt++;
            switch(specifier)
            {
                case '%':
                {
                    if(*buffer == '%') 
                    {
                        continue;
                        ++buffer;
                    }
                    else                
                        error = true;
                    break;
                }
                case 'c':
                {
                    char* p = va_arg(vlist, char*);
                    *p = *buffer;
                    ++buffer;
                    ++ret;
                    break;
                }
                case 's':
                {
                    while (*buffer && isspace(*buffer)) buffer++;
                    char* p = va_arg(vlist, char*);
                    while(*buffer && !isspace(*buffer))
                    {
                        *(p++) = *(buffer++);
                    }
                    *p = '\0';
                    ++ret;
                    break;
                }
                case 'd':
                {
                    int* p = va_arg(vlist, int*);
                    char* end;
                    long num = strtol(buffer, &end, 10);
                    *p = static_cast<int>(num);
                    buffer = end;
                    ++ret;
                    break;
                }
                case 'i':
                {
                    int* p = va_arg(vlist, int*);
                    char* end;
                    long num = strtol(buffer, &end, 0);
                    *p = static_cast<int>(num);
                    buffer = end;
                    ++ret;
                    break;
                }
                case 'u':
                {
                    unsigned int* p = va_arg(vlist, unsigned int*);
                    char* end;
                    long num = strtol(buffer, &end, 10);
                    *p = static_cast<unsigned int>(num);
                    buffer = end;
                    ++ret;
                    break;
                }
                case 'o':
                {
                    unsigned int* p = va_arg(vlist, unsigned int*);
                    char* end;
                    long num = strtol(buffer, &end, 8);
                    *p = static_cast<unsigned int>(num);
                    buffer = end;
                    ++ret;
                    break;
                }
                case 'x':
                case 'X':
                {
                    unsigned int* p = va_arg(vlist, unsigned int*);
                    char* end;
                    long num = strtol(buffer, &end, 16);
                    *p = static_cast<unsigned int>(num);
                    buffer = end;
                    ++ret;
                    break;
                }
                default:
                    break;
            }
        }
    }
    return ret;
}
int sscanf( const char* buffer, const char* format, ... )
{
   	int ret;
	va_list va;
	va_start(va, format);
	ret = vsscanf(buffer, format, va);
	va_end(va);
    return ret; 
}
#ifndef _HHOS_LIBC_TEST
static char scanfbuf[80*25];
//TODO 关于这个返回值
int scanf( const char* format, ... )
{
    if (!gets(scanfbuf)) return EOF;
	int ret;
	va_list va;
	va_start(va, format);
	ret = vsscanf(scanfbuf, format, va);
	va_end(va);
    return ret;
}
#endif

#ifdef _HHOS_LIBC_TEST
}
#endif
