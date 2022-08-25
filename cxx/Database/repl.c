#include <stdio.h>
#include <stdlib.h>

#include "connection.h"
#include "meta_command.h"
#include "prepare.h"
#include "statement.h"
#include "string_buffer.h"
#include "table.h"

#include <stdbool.h>

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

  Table* table = new_table();
  StringBuffer* input = default_buffer();

  while (true) {
    print_prompt();

    read_line(input);

    if (input->buffer[0] == '.') {
      switch (do_meta_command(input)) {
        case OK_SUCCESS:
          continue;

        case ERR_INVALID_COMMAND:
          printf("Commande '%s' invalide\n", input->buffer);
          continue;
      }
    }

    Statement statement;
    switch (prepare_statement(input, &statement)) {
      case OK_SUCCESS:
        break;

      case ERR_INVALID_STATEMENT:
        printf("Instruction '%s' non reconnue.\n", input->buffer);
        continue;

      case ERR_SYNTAX_ERROR:
        printf("Erreur de syntaxe. Impossible d'analyser l'instruction.\n");
        continue;
    }

    switch (execute_statement(&statement, table)) {
      case OK_EXECUTE:
        printf("Exécutée.\n");
        break;

      case ERR_TABLE_FULL:
        printf("Erreur: table pleine.\n");
        break;
    }
  }
}
