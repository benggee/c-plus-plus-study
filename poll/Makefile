CC=g++
CFLAGS=-g

all: poll_server poll_client

poll_server:
	$(CC) $(CFLAGS) -o poll_server poll_server.cpp -lm -lc

poll_client:
	$(CC) $(CFLAGS) -o poll_client poll_client.cpp -lm -lc

clean:
	rm -rf poll_client poll_server
