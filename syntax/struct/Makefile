struct: struct.o main.o 

struct.o: struct.c struct.h
	clang -c struct.c struct.h 
main.o: main.c 
	clang -c main.c struct.h
