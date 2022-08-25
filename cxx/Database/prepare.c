#include <stdio.h>
#include <string.h>

#include "prepare.h"

PrepareResult
prepare_statement(StringBuffer* input, Statement* statement)
{
  if (strncmp(input->buffer, "insert", 6) == 0) {
    statement->type = INSERT;
    int assigned_args = sscanf(input->buffer,
                               "insert %d %s %s",
                               &(statement->row.id),
                               statement->row.username,
                               statement->row.email);

    if (assigned_args < 3) {
      return ERR_SYNTAX_ERROR;
    }

    return OK_PREPARE;
  }

  if (strcmp(input->buffer, "select") == 0) {
    statement->type = SELECT;
    return OK_PREPARE;
  }

  return ERR_INVALID_STATEMENT;
}