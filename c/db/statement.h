#ifndef DB_STATEMENT_H
#define DB_STATEMENT_H

#include "table.h"

typedef enum
{
  OK_EXECUTE,
  ERR_TABLE_FULL
} ExecuteResult;

typedef enum
{
  INSERT,
  SELECT
} StatementType;

typedef struct
{
  StatementType type;
  Row row;
} Statement;

ExecuteResult
execute_statement(Statement* statement, Table* table);

ExecuteResult
execute_insert(Statement* statement, Table* table);

ExecuteResult
execute_select(Statement* statement, Table* table);

#endif
