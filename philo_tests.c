
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

void	*func1(void *arg)
{
		t_all_data *all_data;
		int i;

		all_data = (t_all_data *)arg;
		int right = all_data->index + 1;
		i = 0;
		if (all_data->index == all_data->n_philo - 1)
			right = 0;
		pthread_mutex_lock(&all_data->forks[all_data->index]);
		pthread_mutex_lock(&all_data->forks[right]);
		printf("we ate \n");
		usleep(3);
		pthread_mutex_unlock(&all_data->forks[all_data->index]);
		pthread_mutex_unlock(&all_data->forks[right]);
		return NULL;

}

void	monitor_everything(void *arg)
{
	t_all_data	*all_data;
	all_data = (t_all_data *)arg;
	int first_index;

}

void	init_everything(t_all_data *all_data)
{
	int	i;

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
				all_data->index = i;
		pthread_create(&all_data->philos[i].th, NULL, &func1, all_data);		
		i++;
	}

}

int	main(int ac, char **av)
{
	t_all_data	all_data;
	t_philo		philos[256];
	pthread_t	monitor;
	int			i;

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
