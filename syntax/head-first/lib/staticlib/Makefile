encrypt.o: encrypt.c
	gcc -c encrypt.c -o encrypt.o
checksum.o: checksum.o
	gcc -c checksum.c -o checksum.o
libhfsecurity.a: encrypt.o checksum.o
	ar -rcs libhfsecurity.a encrypt.o checksum.o
test_code: test_code.c libhfsecurity.a
	gcc test_code -I . -L . -lhfsecurity -o test_code
