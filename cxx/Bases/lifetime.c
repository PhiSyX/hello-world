#include <stdio.h>

// Segmentation fault, because lifetime

int*
my_fn(void)
{
  int n = 2;
  return &n;
}

void
another_fn(int b)
{}

int
main(void)
{
  int* ref_n = my_fn();

  another_fn(3);

  int test_n = 4;

  printf("%i\n", *ref_n);
}
