#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct				lopo
{
	pthread_mutex_t	lock;
};
int					a = 0;
void	*func1(void *args)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&((struct lopo *)args)->lock);
	while (i < 500)
	{
		printf("%d\n", i++);
	}
	pthread_mutex_unlock(&((struct lopo *)args)->lock);
	return (NULL);
}

void	*func2(void *args)
{
	int	i;

	i = 0;
	i = 0;
	pthread_mutex_lock(&((struct lopo *)args)->lock);
	while (i < 500)	
	{
		printf("%d\n", i++);
	}
	pthread_mutex_unlock(&((struct lopo *)args)->lock);
	return (NULL);
}

int	main(void)
{
	pthread_t	lopo1;
	pthread_t	lopo2;
	struct lopo	*sopo1;

	sopo1 = malloc(sizeof(struct lopo));
	pthread_mutex_init(&sopo1->lock, NULL);
	pthread_create(&lopo1, NULL, func1, sopo1);
	pthread_create(&lopo2, NULL, func2, sopo1);
	pthread_join(lopo1, NULL);
	pthread_join(lopo2, NULL);
	pthread_mutex_destroy(&sopo1->lock);
	free(sopo1);
	return (0);
}
