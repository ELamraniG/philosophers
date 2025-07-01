#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>

int	main(void)
{
	int i = 10;
	int c = 20;
	sem_t *l;

	int pid1 = fork();
	l = sem_open("/fgefgffgewgwe", 1);
	if (pid1 == 0)
	{

		sem_wait(l);
		printf("child1\n");
		return (0);
	}
	int pid2 = fork();
	if (pid2 == 0)
	{
		
		sem_wait(l);
		printf("child2\n");
		return (0);
	}

}