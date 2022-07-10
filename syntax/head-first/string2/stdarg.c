#include <stdio.h>
#include <stdarg.h>

void print_ints(int args, ...)
{
	va_list ap;
	va_start(ap, args);
	int i;
	for (i=0; i<args; i++) {
		printf("argument: %i\n", va_arg(ap, int));
	}

	va_end(ap);
}

int main()
{
	print_ints(12,3,3,4,22,5,6,6,7);
	return 0;
}
