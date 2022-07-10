CC=g++
CFLAGS=-g -std=c++11

.PHONY: all clean

all: democlass auto-decltype const-volatile-mutable smart_ptr exception

democlass:
	$(CC) $(CFLAGS) -o main main.cpp democlass.hpp -lm -lc

auto-decltype:
	$(CC) $(CFLAGS) -o auto-decltype auto-decltype.cpp -lm -lc

const-volatile-mutable:
	$(CC) $(CFLAGS) -o const-volatile-mutalbe const-volatile-mutable.cpp -lm -lc

smart_ptr:
	$(CC) $(CFLAGS) -o smart_ptr smart_ptr.cpp -lm -lc

clean:
	rm -rf main democlass auto-decltype const-volatile-mutable smart_ptr const-volatile-mutalbe exception
