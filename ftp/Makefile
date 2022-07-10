.PHONY: all clean

ROOT_PATH = ../

ILIB = -I$(ROOT_PATH)/lib
LLIB = -L$(ROOT_PATH)/lib
CC = g++
CFLAGS = -g
CCEXT  = -lmyftp -lm -lc
DEPENDENT_CPP = $(ROOT_PATH)/lib/_ftp.cpp $(ROOT_PATH)/lib/_public.cpp

all: ftpclient

ftpclient: ftpclient.cpp
	$(CC) $(CFLAGS) -o ftpclient ftpclient.cpp $(DEPENDENT_CPP) $(ILIB) $(LLIB) $(CCEXT)

clean:
	-rm ftpclient
