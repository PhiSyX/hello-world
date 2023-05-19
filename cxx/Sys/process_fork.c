
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int
main(int argc, char* argv[])
{
	pid_t pid;
	printf("L'ID du processus principale du programme %d\n", (int)getpid());
	pid = fork();

	if (pid != 0) {
		printf("Il s'agit du processus parent avec l'ID %d\n", (int)getpid());
		printf("L'ID du processus de l'enfant est %d\n", (int)getpid());
	} else {
		printf("Il s'agit du processus enfant avec l'ID %d\n", (int)getpid());
	}

	return 0;
}