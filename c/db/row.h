#ifndef DB_ROW_H
#define DB_ROW_H

#include <stdint.h>

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

typedef struct
{
  uint32_t id;
  char username[COLUMN_USERNAME_SIZE];
  char email[COLUMN_EMAIL_SIZE];
} Row;

#define ID_SIZE sizeof(((Row*)0)->id)
#define USERNAME_SIZE sizeof(((Row*)0)->username)
#define EMAIL_SIZE sizeof(((Row*)0)->email)

#define ID_OFFSET 0
#define USERNAME_OFFSET ID_OFFSET + ID_SIZE
#define EMAIL_OFFSET USERNAME_OFFSET + USERNAME_SIZE
#define ROW_SIZE ID_SIZE + USERNAME_SIZE + EMAIL_SIZE

void
print_row(Row* row);

void
serialize_row(Row* source, void* destination);

void
deserialize_row(void* source, Row* destination);

#endif