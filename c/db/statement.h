#ifndef DB_STATEMENT_H
#define DB_STATEMENT_H

typedef enum
{
  INSERT,
  SELECT
} StatementType;

typedef struct
{
  StatementType type;
} Statement;

void
execute_statement(Statement* statement);

#endif
