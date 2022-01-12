#include "prepare.h"

PrepareResult
prepare_statement(StringBuffer* input, Statement* statement)
{
  if (strncmp(input->buffer, "insert", 6) == 0) {
    statement->type = INSERT;
    return OK_PREPARE;
  }

  if (strcmp(input->buffer, "select") == 0) {
    statement->type = SELECT;
    return OK_PREPARE;
  }

  return ERR_INVALID_STATEMENT;
}