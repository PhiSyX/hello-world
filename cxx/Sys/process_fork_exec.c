#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int
spawn(char* cmd_name, char* args[])
{
	pid_t pid;
	pid = fork();

	if (pid != 0) {
		return pid;
	} else {
		execvp(cmd_name, args);
		fprintf(stderr,
				"Une erreur s'est produite lors de l'exécution du programme\n");
		abort();
	}
}

int
main(int argc, char* argv[])
{
	char* args[] = { "ls", "-l", "/home/phisyx", NULL };
	spawn("ls", args);
	printf("Fin du programme principal\n");
	return 0;
}