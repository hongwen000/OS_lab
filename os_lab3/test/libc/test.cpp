#include <iostream>
#include <gtest/gtest.h>
#include <cstring>
#ifndef _HHOS_LIBC_TEST
#define _HHOS_LIBC_TEST
#endif
#include "./string.h"
#include "./stdio.h"
#include "./stdlib.h"

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

}

int main(int argc, char** argv) {  
    testing::InitGoogleTest(&argc, argv);  
    return RUN_ALL_TESTS();  
}  
