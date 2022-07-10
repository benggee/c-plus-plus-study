CC=g++
CFLAGS=-g

.PHONY: clean all

all: epoll_server epoll_client epoll_server_edge_trigger

epoll_server:
	$(CC) $(CFLAGS) -o epoll_server epoll_server.cpp -lm -lc

epoll_client:
	$(CC) $(CFLAGS) -o epoll_client epoll_client.cpp -lm -lc

epoll_server_edge_trigger:
	$(CC) $(CFLAGS) -o epoll_server_edge_trigger epoll_server_edge_trigger.cpp -lm -lc

clean:
	rm -rf epoll_server epoll_client epoll_server_edge_trigger 
