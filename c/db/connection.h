#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H

typedef struct
{
  int fd;
} Connection;

Connection*
open_connection(char* filename);

#endif