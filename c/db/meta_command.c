#include <stdlib.h>

#include "meta_command.h"

MetaCommand
do_meta_command(StringBuffer* input_buffer)
{
  if (strcmp(input_buffer->buffer, ".exit") == 0) {
    exit(EXIT_SUCCESS);
  } else {
    return ERR_INVALID_COMMAND;
  }
}