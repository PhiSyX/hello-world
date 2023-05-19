#include <stdio.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>

int
main(int argc, char* argv[])
{
	int segid;
	int segsize;

	char* mem_shared;
	const int segsize_shared = 0x6400;

	struct shmid_ds shmbuf;

	segid = shmget(
	  IPC_PRIVATE, segsize_shared, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	mem_shared = (char*)shmat(segid, 0, 0);
	printf("mem_shared attaché a l'adresse %p\n", mem_shared);
	shmctl(segid, IPC_STAT, &shmbuf);
	segsize = shmbuf.shm_segsz;
	printf("segsize: %d\n", segsize);
	sprintf(mem_shared, "Hello, world.");
	shmdt(mem_shared);

	mem_shared = (char*)shmat(segid, (void*)0x5000000, 0);
	printf("mem_shared re-attaché a l'adresse %p\n", mem_shared);
	printf("%s\n", mem_shared);

	shmdt(mem_shared);
	shmctl(segid, IPC_RMID, 0);

	return 0;
}