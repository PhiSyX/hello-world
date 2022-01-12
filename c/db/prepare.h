#ifndef DB_PREPARE_H
#define DB_PREPARE_H

#include "statement.h"
#include "string_buffer.h"

enum Prepare
{
  OK_PREPARE,
  ERR_INVALID_STATEMENT
};

typedef enum Prepare PrepareResult;

PrepareResult
prepare_statement(StringBuffer* input, Statement* statement);

#endif