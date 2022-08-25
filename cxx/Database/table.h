#ifndef DB_TABLE_H
#define DB_TABLE_H

#include <stdint.h>

#include "row.h"

#define TABLE_MAX_PAGES 100
#define PAGE_SIZE 4096
#define ROWS_PER_PAGE PAGE_SIZE / ROW_SIZE
#define TABLE_MAX_ROWS ROWS_PER_PAGE* TABLE_MAX_PAGES

typedef struct
{
  void* pages[TABLE_MAX_PAGES];
  uint32_t num_rows;
} Table;

void*
row_slot(Table* table, uint32_t row_num);

Table*
new_table();

#endif