#include <algorithm>
#include <chrono>
#include <climits>
#include <cmath>
#include <ctime>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <random>
#include <regex>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include <type_traits>
#include <gtest/gtest.h>
#define _HHOS_LIBC_TEST
#include "./string.h"

TEST(string, strlen) {
  EXPECT_EQ(hhlibc::strlen("123"), std::strlen("123"));
  EXPECT_EQ(hhlibc::strlen(""), 0);
}

TEST(string, memcmp) {
    char const xxxxx[] = "xxxxx";
    EXPECT_TRUE( hhlibc::memcmp( "abcde", "abcdx", 5 ) < 0 );
    EXPECT_TRUE( hhlibc::memcmp( "abcde", "abcdx", 4 ) == 0 );
    EXPECT_TRUE( hhlibc::memcmp( "abcde", "xxxxx", 0 ) == 0 );
    EXPECT_TRUE( hhlibc::memcmp( "xxxxx", "abcde", 1 ) > 0 );
}

TEST(string, memcpy) {
    char s[] = "xxxxxxxxxxx";
    EXPECT_TRUE( memcpy( s, "abcde", 6 ) == s );
    EXPECT_TRUE( s[4] == 'e' );
    EXPECT_TRUE( s[5] == '\0' );
    EXPECT_TRUE( memcpy( s + 5, "abcde", 5 ) == s + 5 );
    EXPECT_TRUE( s[9] == 'e' );
    EXPECT_TRUE( s[10] == 'x' );
}

TEST(string, memmove) {
    char s[] = "xxxxabcde";
    EXPECT_TRUE( memmove( s, s + 4, 5 ) == s );
    EXPECT_TRUE( s[0] == 'a' );
    EXPECT_TRUE( s[4] == 'e' );
    EXPECT_TRUE( s[5] == 'b' );
    EXPECT_TRUE( memmove( s + 4, s, 5 ) == s + 4 );
    EXPECT_TRUE( s[4] == 'a' );
}

TEST(string, memset) {
    char s[] = "xxxxxxxxx";
    EXPECT_TRUE( memset( s, 'o', 10 ) == s );
    EXPECT_TRUE( s[9] == 'o' );
    EXPECT_TRUE( memset( s, '_', 0 ) == s );
    EXPECT_TRUE( s[0] == 'o' );
    EXPECT_TRUE( memset( s, '_', 1 ) == s );
    EXPECT_TRUE( s[0] == '_' );
    EXPECT_TRUE( s[1] == 'o' );
}

int main(int argc, char** argv) {  
    testing::InitGoogleTest(&argc, argv);  
  
    return RUN_ALL_TESTS();  
}  