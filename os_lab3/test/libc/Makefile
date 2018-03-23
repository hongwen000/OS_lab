test: build
	./test

build: test.cpp
	clang++ test.cpp -fno-builtin  -I../include -I ../basic_lib -lgtest -o test
