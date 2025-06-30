#include "threads.h"

long	ft_get_time(t_philo *philo)
{
	struct timeval	tm;

	philo = NULL;
	gettimeofday(&tm, NULL);
	return (tm.tv_sec * 1000 + tm.tv_usec / 1000);
}

void	ft_usleep(t_philo *philo, int n)
{
	long	curr;

	curr = ft_get_time(philo);
	while (1337)
	{
		pthread_mutex_lock(&philo->all_data->lets_die_mutex);
		if (philo->all_data->lets_die == 1)
		{
			pthread_mutex_unlock(&philo->all_data->lets_die_mutex);
			return ;
		}
		pthread_mutex_unlock(&philo->all_data->lets_die_mutex);
		usleep(200);
		if (ft_get_time(philo) - curr >= n)
			break ;
	}
}

int	check_data(int ac, char **av, t_all_data *philos)
{
	if (ac != 5 && ac != 6)
	{
		printf("error in the number of args");
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

void	printing_stuff(t_philo *philo, char *s)
{
	pthread_mutex_lock(&philo->all_data->printing);
	pthread_mutex_lock(&philo->all_data->lets_die_mutex);
	if (philo->all_data->lets_die == 1)
	{
		pthread_mutex_unlock(&philo->all_data->lets_die_mutex);
		pthread_mutex_unlock(&philo->all_data->printing);
		return ;
	}
	printf("%lu %d %s\n", ft_get_time(philo) - (philo->all_data->start_time),
		philo->index + 1, s);
	pthread_mutex_unlock(&philo->all_data->lets_die_mutex);
	pthread_mutex_unlock(&philo->all_data->printing);
}

void	*philo_simu(void *arg)
{
	t_philo	*philo;
	int		left;
	int		right;

	philo = (t_philo *)arg;
	left = philo->index;
	if (left == philo->all_data->n_philo - 1)
		right = 0;
	if (left % 2 == 0)
		usleep(philo->all_data->t_t_eat / 2 * 1000);
	while (1)
	{
		pthread_mutex_lock(&philo->all_data->lets_die_mutex);
		if (philo->all_data->lets_die == 1)
		{
			pthread_mutex_unlock(&philo->all_data->lets_die_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->all_data->lets_die_mutex);
		printing_stuff(philo, "is thinking");
		pthread_mutex_lock(&philo->all_data->forks[left]);
		printing_stuff(philo, "has taken a fork");
		printing_stuff(philo, "has taken a fork");
		printing_stuff(philo, "is eating");
		ft_usleep(philo, philo->all_data->t_t_eat);
		pthread_mutex_lock(&philo->last_meal_mutex);
		philo->meals_eaten++;
		philo->last_meal = ft_get_time(philo);
		pthread_mutex_unlock(&philo->last_meal_mutex);
		pthread_mutex_unlock(&philo->all_data->forks[left]);
		printing_stuff(philo, "is sleeping");
		ft_usleep(philo, philo->all_data->t_t_sleep);
	}
	return (NULL);
}

void	*handle_one(void *arg)
{
	t_philo	*philo;
	int		left;

	philo = (t_philo *)arg;
	left = philo->index;
	pthread_mutex_lock(&philo->all_data->lets_die_mutex);
	if (philo->all_data->lets_die == 1)
	{
		pthread_mutex_unlock(&philo->all_data->lets_die_mutex);
		return (NULL);
	}
	pthread_mutex_unlock(&philo->all_data->lets_die_mutex);
	printing_stuff(philo, "is thinking");
	pthread_mutex_lock(&philo->all_data->forks[left]);
	printing_stuff(philo, "has taken a fork");
	ft_usleep(philo, philo->all_data->t_t_eat);
	printing_stuff(philo, "is dead");
	return (NULL);
}

void	*monitor_everything(void *arg)
{
	t_all_data	*all_data;
	int			i;
	long		curr_time;
	int			philos[256];

	all_data = (t_all_data *)arg;
	i = 0;
	while (i < all_data->n_philo)
		philos[i++] = 0;
	i = 0;
	while (1)
	{
		if (i == all_data->n_philo)
			i = 0;
		pthread_mutex_lock(&all_data->philos[i].last_meal_mutex);
		pthread_mutex_lock(&all_data->lets_die_mutex);
		curr_time = ft_get_time(all_data->philos);
		if (all_data->t_t_die <= curr_time - all_data->philos[i].last_meal)
		{
			printf("%lu %d %s\n", ft_get_time(&all_data->philos[i])
				- (all_data->start_time), all_data->philos[i].index + 1,
				"is dead");
			all_data->lets_die = 1;
			pthread_mutex_unlock(&all_data->philos[i].last_meal_mutex);
			pthread_mutex_unlock(&all_data->lets_die_mutex);
			return (NULL);
		}
		if (all_data->philos[i].meals_eaten == all_data->meals_to_eat
			&& !philos[i])
		{
			all_data->global_meals_eaten++;
			philos[i] = 1;
		}
		if (all_data->global_meals_eaten == all_data->n_philo)
		{
			all_data->lets_die = 1;
			pthread_mutex_unlock(&all_data->philos[i].last_meal_mutex);
			pthread_mutex_unlock(&all_data->lets_die_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&all_data->philos[i].last_meal_mutex);
		pthread_mutex_unlock(&all_data->lets_die_mutex);
		usleep(200);
		i++;
	}
	return (NULL);
}

void	init_everything(t_all_data *all_data, pthread_mutex_t *forks,
		pthread_mutex_t *last_meals)
{
	int				i;
	struct timeval	tm;

	all_data->forks = forks;
	all_data->global_meals_eaten = 0;
	pthread_mutex_init(&all_data->printing, NULL);
	pthread_mutex_init(&all_data->lets_die_mutex, NULL);
	all_data->lets_die = 0;
	i = 0;
	while (i < all_data->n_philo)
	{
		pthread_mutex_init(&all_data->forks[i], NULL);
		pthread_mutex_init(&last_meals[i], NULL);
		i++;
	}
	i = 0;
	while (i < all_data->n_philo)
	{
		all_data->philos[i].index = i;
		all_data->philos[i].meals_eaten = 0;
		all_data->philos[i].all_data = all_data;
		i++;
	}
	i = 0;
	gettimeofday(&tm, NULL);
	all_data->start_time = tm.tv_sec * 1000 + tm.tv_usec / 1000;
	i = 0;
	if (all_data->n_philo == 1)
	{
		all_data->philos[i].last_meal = all_data->start_time;
		all_data->philos[i].last_meal_mutex = last_meals[i];
		pthread_create(&all_data->philos[i].th, NULL, &handle_one,
			&all_data->philos[i]);
	}
	else
	{
		while (i < all_data->n_philo)
		{
			all_data->philos[i].last_meal = all_data->start_time;
			all_data->philos[i].last_meal_mutex = last_meals[i];
			pthread_create(&all_data->philos[i].th, NULL, &philo_simu,
				&all_data->philos[i]);
			i++;
		}
	}
}

int	main(int ac, char **av)
{
	t_all_data all_data;
	t_philo philos[256];
	pthread_mutex_t forks[256];
	pthread_mutex_t last_meals[256];
	pthread_t monitor;
	int i;

	i = 0;
	all_data.philos = philos;
	if (check_data(ac, av, &all_data) == -1)
		return (1);
	init_everything(&all_data, forks, last_meals);
	if (all_data.n_philo != 1)
		pthread_create(&monitor, NULL, &monitor_everything, &all_data);
	while (i < all_data.n_philo)
	{
		pthread_join(all_data.philos[i].th, NULL);
		i++;
	}
	pthread_join(monitor, NULL);
	i = 0;
	while (i < all_data.n_philo)
	{
		pthread_mutex_destroy(&all_data.forks[i]);
		pthread_mutex_destroy(&all_data.forks[i]);
		pthread_mutex_destroy(&all_data.philos[i].last_meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&all_data.printing);
	pthread_mutex_destroy(&all_data.lets_die_mutex);
	return (0);
}