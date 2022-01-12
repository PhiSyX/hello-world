#include <stdbool.h>

#include "connection.c"
#include "string_buffer.c"

char*
get_database_filename(int argc, char* argv[])
{
  if (argc < 2) {
    printf("Erreur: nom de fichier invalide pour la base de données.\n");
    exit(EXIT_FAILURE);
  }

  return argv[1];
}

void
print_prompt()
{
  printf("C: db > ");
}

int
main(int argc, char* argv[])
{
  char* database_filename = get_database_filename(argc, argv);

  Connection* connection = open_connection(database_filename);
  StringBuffer* input = default_buffer();

  while (true) {
    print_prompt();

    read_line(input);

    if (strcmp(input->buffer, ".exit") == 0) {
      exit(EXIT_SUCCESS);
    } else {
      printf("La commande '%s' est invalide.\n", input->buffer);
    }
  }
}
