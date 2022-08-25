#include <stdio.h>

/// Octet en valeur hexa.
void
print_bytes(int n_bytes, unsigned char* p_bytes);

int
main(int argc, char* argv[])
{
  char chr = 0x41;        // 1 byte
  short sht = 0x41;       // little/big endian?
  int num = 0x41;         // little/big endian?
  long lng = 0x41;        // little/big endian?
  float flt = 8.0f;       // float format
  double dbl = 131072.0;  // double format
  long double ldbl = 0.0; // long double format

  unsigned char* p_ldbl = (unsigned char*)&ldbl;
  p_ldbl[7] = 0x41;

  printf("       char: %8c    ", chr);
  print_bytes(sizeof(chr), (unsigned char*)&chr);

  printf("      short: %8d    ", sht);
  print_bytes(sizeof(sht), (unsigned char*)&sht);

  printf("        int: %8d    ", num);
  print_bytes(sizeof(num), (unsigned char*)&num);

  printf("       long: %8ld    ", lng);
  print_bytes(sizeof(lng), (unsigned char*)&lng);

  printf("      float: %8.1f    ", flt);
  print_bytes(sizeof(flt), (unsigned char*)&flt);

  printf("     double: %8.1f    ", dbl);
  print_bytes(sizeof(dbl), (unsigned char*)&dbl);

  printf("long double: % 10.2Le ", ldbl);
  print_bytes(sizeof(ldbl), (unsigned char*)&ldbl);

  printf("\n");
}

void
print_bytes(int n_bytes, unsigned char* p_bytes)
{
  for (int i = 0; i < n_bytes; i++) {
    printf("%02x ", p_bytes[i]);
  }

  printf("\n");
}