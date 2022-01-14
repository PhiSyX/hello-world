#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "string_buffer.h"

StringBuffer*
default_buffer()
{
  StringBuffer* input = malloc(sizeof(StringBuffer));
  input->buffer = NULL;
  input->buffer_size = 0;
  input->input_size = 0;
  return input;
}

void
read_line(StringBuffer* input)
{
  ssize_t bytes = getline(&input->buffer, &input->buffer_size, stdin);

  if (bytes <= 0) {
    printf("Erreur lors de la lecture du buffer\n");
    exit(EXIT_FAILURE);
  }

  input->input_size = bytes - 1;
  input->buffer[bytes - 1] = 0;
}
