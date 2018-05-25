#include <iostream>
#include <gtest/gtest.h>
#include <cstring>
#ifndef _HHOS_LIBC_TEST
#define _HHOS_LIBC_TEST
#endif
#include "./string.h"
#include "./stdio.h"
#include "./stdlib.h"
#include "./ctype.h"
#include "./assert.h"

TEST(string, strlen) {
  EXPECT_EQ(hhlibc::strlen("123"), std::strlen("123"));
  EXPECT_EQ(hhlibc::strlen(""), 0);
}

TEST(string, memcmp) {
    EXPECT_TRUE( hhlibc::memcmp( "abcde", "abcdx", 5 ) < 0 );
    EXPECT_TRUE( hhlibc::memcmp( "abcde", "abcdx", 4 ) == 0 );
    EXPECT_TRUE( hhlibc::memcmp( "abcde", "xxxxx", 0 ) == 0 );
    EXPECT_TRUE( hhlibc::memcmp( "xxxxx", "abcde", 1 ) > 0 );
    EXPECT_TRUE( hhlibc::memcmp( "xxxxx", "abcde", 1 ) > 0 );
}

TEST(string, memcpy) {
    char s[] = "xxxxxxxxxxx";
    EXPECT_TRUE( hhlibc::memcpy( s, "abcde", 6 ) == s );
    EXPECT_TRUE( s[4] == 'e' );
    EXPECT_TRUE( s[5] == '\0' );
    EXPECT_TRUE( hhlibc::memcpy( s + 5, "abcde", 5 ) == s + 5 );
    EXPECT_TRUE( s[9] == 'e' );
    EXPECT_TRUE( s[10] == 'x' );
}

TEST(string, memmove) {
    char s[] = "xxxxabcde";
    EXPECT_TRUE( hhlibc::memmove( s, s + 4, 5 ) == s );
    EXPECT_TRUE( s[0] == 'a' );
    EXPECT_TRUE( s[4] == 'e' );
    EXPECT_TRUE( s[5] == 'b' );
    EXPECT_TRUE( hhlibc::memmove( s + 4, s, 5 ) == s + 4 );
    EXPECT_TRUE( s[4] == 'a' );
}

TEST(string, memset) {
    char s[] = "xxxxxxxxx";
    EXPECT_TRUE( hhlibc::memset( s, 'o', 10 ) == s );
    EXPECT_TRUE( s[9] == 'o' );
    EXPECT_TRUE( hhlibc::memset( s, '_', 0 ) == s );
    EXPECT_TRUE( s[0] == 'o' );
    EXPECT_TRUE( hhlibc::memset( s, '_', 1 ) == s );
    EXPECT_TRUE( s[0] == '_' );
    EXPECT_TRUE( s[1] == 'o' );
}

TEST(string, strcmp) {
    EXPECT_TRUE( hhlibc::strcmp( "abcde", "cmpabcde" ) != 0 );
    EXPECT_TRUE( hhlibc::strcmp( "abcde", "abcdx" ) < 0 );
    EXPECT_TRUE( hhlibc::strcmp( "abcdx", "abcde" ) > 0 );
    EXPECT_TRUE( hhlibc::strcmp( "empty", "abcde" ) > 0 );
    EXPECT_TRUE( hhlibc::strcmp( "abcde", "empty" ) < 0 );
    EXPECT_TRUE( hhlibc::strcmp( "abcde", "cmpabcd_" ) < 0 );
}
TEST(string, strcpy) {
    char s[] = "xxxxx";
    EXPECT_TRUE( hhlibc::strcpy( s, "" ) == s );
    EXPECT_TRUE( s[0] == '\0' );
    EXPECT_TRUE( s[1] == 'x' );
    EXPECT_TRUE( hhlibc::strcpy( s, "abcde" ) == s );
    EXPECT_TRUE( s[0] == 'a' );
    EXPECT_TRUE( s[4] == 'e' );
    EXPECT_TRUE( s[5] == '\0' );
}

TEST(string, itoa) {
    char buf[128];
    hhlibc::itoa<int>(buf, -123, 10);
    EXPECT_EQ(std::string(buf), std::string("-123"));
//    hhlibc::itoa(buf, 0, 10);
//    EXPECT_EQ(std::string(buf), std::string("0"));
//    hhlibc::itoa(buf, 7354, 10);
//    EXPECT_EQ(std::string(buf), std::string("7354"));
//    hhlibc::itoa(buf, 16*1 + 10, 16);
//    EXPECT_EQ(std::string(buf), std::string("1A"));
}

TEST(stdio, sprintf) {
    char buf[128];
    hhlibc::sprintf(buf, "%c%d%s%o%x",'a', 123, "string", 16, 15);
    EXPECT_EQ(std::string(buf), std::string("a123string20f"));
}

TEST(stdlib, strtol) {
    char* str = "123";
    auto ret = hhlibc::strtol(str, NULL, 10);
    EXPECT_EQ(ret, 123);
}

TEST(stdlib, div) {
    auto ret = hhlibc::div(11, 3);
    EXPECT_EQ(ret.quot, 3);
    EXPECT_EQ(ret.rem, 2);
}
TEST(stdio, sscanf) {
    char* buf="a123string 20f";
    char c;
    int  n1;
    char s[10];
    int n2;
    int n3;
    hhlibc::sscanf(buf, "%c%d%s%o%x", &c, &n1, s, &n2, &n3);
    EXPECT_EQ(c, 'a');
    EXPECT_EQ(n1, 123);
    EXPECT_EQ(std::string(s), std::string("string"));
    EXPECT_EQ(n2, 16);
    EXPECT_EQ(n3, 15);

    char * sys_internal_time_str = "2018/4/12-23:39:57";
    int sys_internal_second = 0;
    int sys_internal_minute = 0;
    int sys_internal_hour = 0;
    int sys_internal_day = 0;
    int sys_internal_month = 0;
    int sys_internal_year = 0;
    sscanf(sys_internal_time_str,"%d/%d/%d-%d:%d:%d", 
            &sys_internal_year, &sys_internal_month, &sys_internal_day,
            &sys_internal_hour, &sys_internal_minute, &sys_internal_second);
    EXPECT_EQ(sys_internal_year, 2018);
    EXPECT_EQ(sys_internal_month, 4);
    EXPECT_EQ(sys_internal_day, 12);
    EXPECT_EQ(sys_internal_hour, 23);
    EXPECT_EQ(sys_internal_minute, 39);
    EXPECT_EQ(sys_internal_second, 57);
}

int main(int argc, char** argv) {  
    testing::InitGoogleTest(&argc, argv);
    hhos_assert(1 == 2);
    char record_buf[] = "5\npopup-1\n5";
    char buf1[32];
    char buf2[32];
    char buf3[32];
    hhlibc::sscanf(record_buf, "%s%s%s", buf1, buf2, buf3);
    std::cout << buf1 << '\t' <<hhlibc::strlen(buf1) << std::endl;
    std::cout << buf2 << '\t' <<hhlibc::strlen(buf2) << std::endl;
    std::cout << buf3 << '\t' <<hhlibc::strlen(buf3) << std::endl;

    char * p = record_buf;
    char one_record[32];
    hhlibc::sscanf(p, "%s", one_record);
    size_t n = hhlibc::strtol(one_record, NULL, 10);
    std::cout << n << std::endl;
    size_t len = strlen(one_record);
    p+=len+1;
    char name[32];
    char lba[32];
    hhlibc::sscanf(p, "%s%s", name, lba);
    std::cout << name << std::endl;
    std::cout << lba << std::endl;
    return RUN_ALL_TESTS();
}  
