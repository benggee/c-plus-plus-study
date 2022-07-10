CC=g++
CFLAGS=-g
LIBI=-I../lib
PUBL=../lib/_public.cpp

.PHONY: all clean

all: server client

server:
	$(CC) $(CFLAGS) -o server server.cpp $(LIBI) $(PUBL) -lm -lc

client:
	$(CC) $(CFLAGS) -o client client.cpp $(LIBI) $(PUBL) -lm -lc

clean:
	rm server client
