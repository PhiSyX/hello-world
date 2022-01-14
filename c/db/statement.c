#include <stdio.h>

#include "statement.h"
#include "table.h"

ExecuteResult
execute_statement(Statement* statement, Table* table)
{
  switch (statement->type) {
    case INSERT:
      return execute_insert(statement, table);

    case SELECT:
      return execute_select(statement, table);
  }

  return ERR_TABLE_FULL;
}

ExecuteResult
execute_insert(Statement* statement, Table* table)
{
  if (table->num_rows >= TABLE_MAX_ROWS) {
    return ERR_TABLE_FULL;
  }

  Row* row = &statement->row;
  serialize_row(row, row_slot(table, table->num_rows));
  table->num_rows += 1;

  return OK_EXECUTE;
}

ExecuteResult
execute_select(Statement* statement, Table* table)
{
  Row row;
  for (uint32_t i = 0; i < table->num_rows; i++) {
    deserialize_row(row_slot(table, i), &row);
    print_row(&row);
  }
  return OK_EXECUTE;
}
