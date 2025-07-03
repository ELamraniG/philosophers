#include "threads.h"

long	ft_get_time(void)
{
	struct timeval	tm;

	gettimeofday(&tm, NULL);
	return (tm.tv_sec * 1000 + tm.tv_usec / 1000);
}

void	ft_usleep(t_philo *philo, int n)
{
	long	curr;

	curr = ft_get_time();
	while (1337)
	{
		sem_wait(philo->all_data->lets_die_sem);
		if (philo->all_data->lets_die == 1)
		{
			sem_post(philo->all_data->lets_die_sem);
			return ;
		}
		sem_post(philo->all_data->lets_die_sem);
		usleep(200);
		if (ft_get_time() - curr >= n)
			break ;
	}
}

int	check_data(int ac, char **av, t_all_data *philos)
{
	if (ac != 5 && ac != 6)
	{
		printf("error in the number of args\n");
		return (-1);
	}
	philos->n_philo = ft_atoi(av[1]);
	philos->t_t_die = ft_atoi(av[2]);
	philos->t_t_eat = ft_atoi(av[3]);
	philos->t_t_sleep = ft_atoi(av[4]);
	philos->meals_to_eat = -2;
	if (ac == 6)
		philos->meals_to_eat = ft_atoi(av[5]);
	if (philos->n_philo == -1 || philos->t_t_die == -1)
		return (-1);
	if (philos->t_t_eat == -1 || philos->t_t_sleep == -1
		|| philos->meals_to_eat == -1)
		return (-1);
	if (philos->n_philo == 0 || philos->n_philo > 200)
	{
		return (-1);
	}
	return (0);
}

void	init_everything(t_all_data *all_data)
{
	int	i;

	sem_unlink("/forks_sem");
	sem_unlink("/last_meal_sem");
	sem_unlink("/printing_sem");
	sem_unlink("/lets_die_sem");
	all_data->lets_die = 0;
	all_data->forks_sem = sem_open("/forks_sem", all_data->n_philo);
	all_data->last_meal_sem = sem_open("/last_meal_sem", 1);
	all_data->printing_sem = sem_open("/printing_sem", 1);
	all_data->lets_die_sem = sem_open("/lets_die_sem", 1);
	all_data->global_meals_eaten = 0;
	i = 0;
	while (i < all_data->n_philo)
	{
		all_data->philos[i].index = i;
		all_data->philos[i].meals_eaten = 0;
		all_data->philos[i].all_data = all_data;
		i++;
	}
}

void	printing_stuff(t_philo *philo, char *s)
{
	sem_wait(philo->all_data->printing_sem);
	sem_wait(philo->all_data->lets_die_sem);
	if (philo->all_data->lets_die == 1)
	{
		sem_post(philo->all_data->printing_sem);
		sem_post(philo->all_data->lets_die_sem);
		return ;
	}
	printf("%lu %d %s\n", ft_get_time() - (philo->all_data->start_time),
		philo->index + 1, s);
	sem_post(philo->all_data->printing_sem);
	sem_post(philo->all_data->lets_die_sem);
}
void	monitor_stuff(void *args)
{
}

void	do_the_philo(t_philo *philos)
{
	pthread_t	monitor;

	pthread_create(&monitor, &monitor_stuff, philos, NULL);
	pthread_detach(monitor);
	printing_stuff(philos, "is thinking");
	sem_wait(philos->all_data->forks_sem);
	printing_stuff(philos, "has taken a fork");
	sem_wait(philos->all_data->forks_sem);
	printing_stuff(philos, "has taken a fork");
	ft_usleep(philos, philos->all_data->t_t_eat);
	sem_wait(philos->all_data->last_meal_sem);
	philos->last_meal = ft_get_time();

}
int	main(int ac, char **av)
{
	t_philo philos[256];
	t_all_data all_data;
	all_data.philos = philos;
	int all_pids[201];
	if (check_data(ac, av, &all_data) == -1)
		return (0);
	init_everything(&all_data);
	int i = 0;
	all_data.start_time = ft_get_time();
	while (i < all_data.n_philo)
		philos[i++].last_meal = all_data.start_time;
	i = 0;
	while (i < all_data.n_philo)
	{
		all_pids[i] = fork();
		if (all_pids[i] == 0)
		{
			do_the_philo(&philos[i]);
			return (0);
		}
	}
}