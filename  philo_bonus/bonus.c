/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 17:53:18 by moel-amr          #+#    #+#             */
/*   Updated: 2025/07/06 18:02:23 by moel-amr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "threads.h"

void	*monitor_stuff(void *args)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)args;
	while (1)
	{
		sem_wait(philo->last_meal_sem);
		if (philo->all_data->t_t_die <= ft_get_time() - philo->last_meal)
		{
			sem_wait(philo->all_data->printing_sem);
			printf("%lu %d %s\n", ft_get_time() - (philo->all_data->start_time),
				philo->index + 1, "died");
			i = 0;
			while (i < philo->all_data->n_philo)
			{
				sem_post(philo->all_data->meals_eaten_sem);
				i++;
			}
		}
		if (philo->meals_eaten == philo->all_data->meals_to_eat)
			sem_post(philo->all_data->meals_eaten_sem);
		sem_post(philo->last_meal_sem);
		usleep(200);
	}
}

void	do_the_philo(t_philo *philos)
{
	pthread_t	monitor;

	pthread_create(&monitor, NULL, &monitor_stuff, philos);
	pthread_detach(monitor);
	while (1337)
	{
		printing_stuff(philos, "is thinking");
		sem_wait(philos->all_data->forks_sem);
		printing_stuff(philos, "has taken a fork");
		sem_wait(philos->all_data->forks_sem);
		printing_stuff(philos, "has taken a fork");
		printing_stuff(philos, "is eating");
		usleep(philos->all_data->t_t_eat * 1000);
		sem_wait(philos->last_meal_sem);
		philos->last_meal = ft_get_time();
		philos->meals_eaten++;
		sem_post(philos->last_meal_sem);
		sem_post(philos->all_data->forks_sem);
		sem_post(philos->all_data->forks_sem);
		printing_stuff(philos, "is sleeping");
		usleep(1000 * philos->all_data->t_t_sleep);
	}
	return ;
}

void	clean_exit(t_all_data *all_data, int *all_pids)
{
	int	i;

	i = 0;
	while (i < all_data->n_philo)
	{
		sem_wait(all_data->meals_eaten_sem);
		i++;
	}
	i = 0;
	while (i < all_data->n_philo)
	{
		kill(all_pids[i], SIGKILL);
		waitpid(all_pids[i], NULL, 0);
		sem_close(all_data->philos[i].last_meal_sem);
		i++;
	}
	sem_close(all_data->forks_sem);
	sem_close(all_data->printing_sem);
	sem_close(all_data->meals_eaten_sem);
}
int	main(int ac, char **av)
{
	t_philo		philos[256];
	t_all_data	all_data;
	int			all_pids[201];
	int			i;

	all_data.philos = philos;
	if (check_data(ac, av, &all_data) == -1)
		return (0);
	init_everything(&all_data);
	i = 0;
	all_data.start_time = ft_get_time();
	while (i < all_data.n_philo)
		philos[i++].last_meal = all_data.start_time;
	i = 0;
	while (i < all_data.n_philo)
	{
		all_pids[i] = fork();
		if (all_pids[i] == 0)
			do_the_philo(&philos[i]);
		i++;
	}
	clean_exit(&all_data, all_pids);
	return (0);
}
