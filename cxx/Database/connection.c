#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "connection.h"

Connection*
open_connection(char* filename)
{
  int fd = open(filename, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
  if (fd == -1) {
    printf("Impossible d'ouvrir le fichier '%s'\n", filename);
    exit(EXIT_FAILURE);
  }

  Connection* connection = malloc(sizeof(Connection));
  connection->fd = fd;
  return connection;
}
