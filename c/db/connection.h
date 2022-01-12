#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
  int fd;
} Connection;

Connection*
open_connection(char* filename);

#endif