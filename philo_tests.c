#include "threads.h"

int	check_data(int ac, char **av, t_all_data *philos)
{
	if (ac != 5 && ac != 6)
	{
		printf("error in the number of args");
		return (1);
	}
	// change atoi to ft atoi
	philos->n_philo = atoi(av[1]);
	philos->t_t_die = atoi(av[2]);
	philos->t_t_eat = atoi(av[3]);
	philos->t_t_sleep = atoi(av[4]);
	// do this later if (ac == 6)
	return (0);
}

void get_the_damn_time(t_philo *philo)
{
		struct timeval tm;
		gettimeofday(&tm,NULL);
		philo->last_meal =   (tm.tv_sec * 1000 + tm.tv_usec / 1000) - (philo->all_data->start_time);
}

void	*func1(void *arg)
{
	t_philo		*philo;
	t_all_data	*all_data;
	int			left;
	int			right;
	int			i;

	philo = (t_philo *)arg;
	all_data = philo->all_data;
	left = philo->index;
	right = left + 1;
	if (left == all_data->n_philo - 1)
		right = 0;
	i = 0;
	while (1)
	{
		printf("%d is thinking\n", left);
		if (left % 2 == 0)
		{
			pthread_mutex_lock(&all_data->forks[left]);
			printf("%d has taken a fork\n" ,left);
			pthread_mutex_lock(&all_data->forks[right]);
			printf("%d has taken a fork\n" ,left);
		}
		else
		{
			pthread_mutex_lock(&all_data->forks[right]);
			printf("%lu %d has taken a fork\n",philo->last_meal ,left);
			pthread_mutex_lock(&all_data->forks[left]);
			printf("%lu %d has taken a fork\n",philo->last_meal ,left);

		}
		get_the_damn_time(philo);
		printf("%lu %d is eating\n",philo->last_meal, left);
		pthread_mutex_unlock(&all_data->forks[left]);
		pthread_mutex_unlock(&all_data->forks[right]);
		get_the_damn_time(philo);
		printf("%lu %d is sleeping\n",philo->last_meal , left);
		usleep(philo->all_data->t_t_sleep);
		i++;
	}
	return (NULL);
}

void	monitor_everything(void *arg)
{
	t_all_data	*all_data;

	// int			first_index;
	all_data = (t_all_data *)arg;
}

void	init_everything(t_all_data *all_data)
{
	int	i;
	struct timeval tm;

	all_data->forks = malloc(sizeof(pthread_mutex_t) * all_data->n_philo);
	pthread_mutex_init(&all_data->printing, NULL);
	pthread_mutex_init(&all_data->dead_lock, NULL);
	i = 0;
	while (i < all_data->n_philo)
	{
		pthread_mutex_init(&all_data->forks[i], NULL);
		i++;
	}
	i = 0;
	while (i < all_data->n_philo)
	{
		all_data->philos[i].index = i;
		all_data->philos[i].all_data = all_data;
		i++;
	}
		i = 0;
		gettimeofday(&tm,NULL);
		all_data->start_time = tm.tv_sec * 1000 + tm.tv_usec / 1000;
	while (i < all_data->n_philo)
	{
		all_data->philos[i].last_meal = 0;
		pthread_create(&all_data->philos[i].th, NULL, &func1,
			&all_data->philos[i]);
		i++;
	}
}

int	main(int ac, char **av)
{
	t_all_data all_data;
	t_philo philos[256];
	// pthread_t monitor;
	int i;

	i = 0;
	all_data.philos = philos;
	if (check_data(ac, av, &all_data) == 1)
		return (1);
	init_everything(&all_data);
	// thread_create(monitor, NULL, &monitor_everything, all_data);
	while (i < all_data.n_philo)
	{
		pthread_join(all_data.philos[i].th, NULL);
		i++;
	}
	// pthread_join(&monitor, NULL);
	return (0);
}