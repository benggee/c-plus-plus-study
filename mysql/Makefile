MYSQLINCL=-I/usr/local/mysql/include
MYSQLLIB=-L/usr/local/mysql/lib
MYSQLLIBS=-lmysqlclient

CFLAGS=-g
CC=g++

.PHONY: all clean

all: createtable insertdata selectdata deletedata

createtable: createtable.cpp ../lib/_mysql.h ../lib/_mysql.cpp
	$(CC) $(CFLAGS) -o createtable createtable.cpp $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) ../lib/_mysql.cpp -lm -lc

insertdata: insertdata.cpp ../lib/_mysql.h ../lib/_mysql.cpp
	$(CC) $(CFLAGS) -o insertdata insertdata.cpp $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) ../lib/_mysql.cpp -lm -lc

selectdata: selectdata.cpp ../lib/_mysql.h ../lib/_mysql.cpp
	$(CC) $(CFLAGS) -o selectdata selectdata.cpp $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) ../lib/_mysql.cpp -lm -lc

deletedata: deletedata.cpp ../lib/_mysql.h ../lib/_mysql.cpp
	$(CC) $(CFLAGS) -o deletedata deletedata.cpp $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) ../lib/_mysql.cpp -lm -lc

clean:
	rm -rf createtable insertdata selectdata deletedata 
