#include <sys/shm.h>

#include <err.h>
#include <stdio.h>
#include <string.h>

#define QUOTE(s) 	#s
#define SHM_SIZE	512
#define STR(s)		QUOTE(s)

unsigned char shellcode[] = STR(SHELLCODE);
unsigned char stage2[] = STR(STAGE2);

int
main(void)
{
	int	 shmid;
	void	*addr;

	if ((shmid = shmget(0xcafebabe, SHM_SIZE, 0644|IPC_CREAT)) == -1)
		err(1, "shmget");
	if ((addr = shmat(shmid, NULL, 0)) == (void *) -1)
		err(1, "shmat");
	memcpy(addr, stage2, SHM_SIZE);

	printf("Shellcode length: %zu\n", sizeof shellcode - 1);
	printf("Second stage length: %zu\n", sizeof stage2 - 1);
	(*(void(*)(void)) shellcode)();

	return (0);
}
