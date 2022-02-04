#ifndef DB_META_COMMAND_H
#define DB_META_COMMAND_H

#include "string_buffer.h"

typedef enum
{
  OK_SUCCESS,
  ERR_INVALID_COMMAND
} MetaCommand;

MetaCommand
do_meta_command(StringBuffer* input);

#endif