#ifndef DB_STRING_BUFFER_H
#define DB_STRING_BUFFER_H

#include <sys/types.h>

typedef struct
{
  char* buffer;
  size_t buffer_size;
  ssize_t input_size;
} StringBuffer;

StringBuffer*
default_buffer();

void
read_line(StringBuffer* input);

#endif