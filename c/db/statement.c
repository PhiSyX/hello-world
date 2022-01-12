#include <stdio.h>

#include "statement.h"

void
execute_statement(Statement* statement)
{
  switch (statement->type) {
    case INSERT:
      printf("Instruction: INSERT\n");
      break;

    case SELECT:
      printf("Instruction: SELECT\n");
      break;
  }
}