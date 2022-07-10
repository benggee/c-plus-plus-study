CC=g++
CFLAGS=-g

.PHONY: all clean

all: forward-proxy reverse-proxy-in reverse-proxy-out

forward-proxy:
	$(CC) $(CFLAGS) -o forward-proxy forward-proxy.cpp ../../lib/_public.cpp -lm -lc

reverse-proxy-in:
	$(CC) $(CFLAGS) -o reverse-proxy-in reverse-proxy-in.cpp ../../lib/_public.cpp -lm -lc

reverse-proxy-out:
	$(CC) $(CFLAGS) -o reverse-proxy-out reverse-proxy-out.cpp ../../lib/_public.cpp -lm -lc

clean: 
	rm -rf forward-proxy reverse-proxy-in reverse-proxy-out
