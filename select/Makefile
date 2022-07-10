CC=g++
CFLAGS=-g

.PHONY: all clean

all: select_server select_client

select_server: 
	$(CC) $(CFLAGS) -o select_server select_server.cpp -lm -lc

select_client:
	$(CC) $(CFLAGS) -o select_client select_client.cpp -lm -lc

clean:
	rm -rf select_server select_client
