#include "../lib/common.h"
#include <string.h>
#include <stdlib.h>

int main() {
  char *mem = malloc(10);
  memset(mem, 0, 10);

  PRINT_INT_ARRAY(mem, 10);
  free(mem);

  char *left = "Hello World!";
  char *right = "Hello C Programmers!";

  PRINT_INT(strcmp(left, right));
  PRINT_INT(strncmp(left, right, 5));

  char src[] = "HelloWorld";
  char *dest = malloc(11);
  memset(dest, 0, 11);
  memcpy(dest, src, 11);
  puts(dest);
  memcpy(dest + 3, dest + 1, 4);
  puts(dest);
  free(dest);
  return 0;
}
