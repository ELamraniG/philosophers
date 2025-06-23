#ifndef THREADS_H
# define THREADS_H
#endif

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 #include <sys/time.h>

typedef struct s_philo t_philo;
typedef struct s_all_data
{
	int				n_philo;
	long			t_t_die;
	long			t_t_eat;
	long			t_t_sleep;
	int				meals_to_drink;
	long			start_timing;
	int				dead;
	pthread_mutex_t	*forks;
	pthread_mutex_t	printing;
	pthread_mutex_t	dead_lock;
	t_philo			*philos;
}					t_all_data;


typedef struct s_philo
{
	long			meals_swallowed;
	long			last_meal;
	pthread_t		th;
	int			
		index;
	t_all_data		*all_data;
}					t_philo;

