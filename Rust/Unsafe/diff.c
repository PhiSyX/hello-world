#include <stdio.h>

int
foo(int* restrict x, int* restrict y)
{
  *x = 42;
  *y = 7;
  return *x;
}

int
bar(int* x, int* y)
{
  *x = 42;
  *y = 7;
  return *x;
}

int
main()
{
  int a = 0;
  printf("Output 1: %d\n", foo(&a, &a));

  int b = 0;
  printf("Output 2: %d\n", bar(&b, &b));
}

/*
$ cc -o diff.o diff.c
$ ./diff

Output 1: 7
Output 2: 7
*/

/*
$ cc -o diff.o diff.c -O3
$ ./diff

Output 1: 42
Output 2: 7
*/
