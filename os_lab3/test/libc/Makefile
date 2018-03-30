test: build
	./test

build: test.cpp
	clang++ -Wno-writable-strings -fno-builtin -D_HHOS_LIBC_TEST -I../include -I ../basic_lib -lgtest ctype.cpp stdlib.cpp stdio.cpp string.cpp test.cpp -g -std=c++1z  -o test
