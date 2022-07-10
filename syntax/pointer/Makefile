pointer: pointer.o main.o 

pointer.o: pointer.c pointer.h
	clang -c pointer.c pointer.h 
main.o: main.c 
	clang -c main.c pointer.h
