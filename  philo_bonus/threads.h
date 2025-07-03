#ifndef THREADS_H
# define THREADS_H

# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdio.h>
# include <unistd.h>

typedef struct s_all_data
{
	sem_t		*forks_sem;
	sem_t		*printing_sem;
	sem_t		*lets_die_sem;
	int			n_philo;
	long		t_t_die;
	long		t_t_eat;
	long		t_t_sleep;
	int			lets_die;
	int			meals_to_eat;
	long		global_meals_eaten;
	long		start_time;
	t_philo		*philos;
}				t_all_data;

typedef struct s_philo
{
	sem_t		*last_meal_sem;
	long		meals_eaten;
	long		last_meal;
	int			index;
	t_all_data	*all_data;
}				t_philo;
int				ft_atoi(const char *s);

#endif