/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 17:48:59 by moel-amr          #+#    #+#             */
/*   Updated: 2025/07/06 18:08:21 by moel-amr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "threads.h"

int	check_data(int ac, char **av, t_all_data *all_data)
{
	if (ac != 5 && ac != 6)
	{
		printf("error in the number of args\n");
		return (-1);
	}
	all_data->n_philo = ft_atoi(av[1]);
	all_data->t_t_die = ft_atoi(av[2]);
	all_data->t_t_eat = ft_atoi(av[3]);
	all_data->t_t_sleep = ft_atoi(av[4]);
	all_data->meals_to_eat = -2;
	if (ac == 6)
		all_data->meals_to_eat = ft_atoi(av[5]);
	if (all_data->n_philo == -1 || all_data->t_t_die == -1)
		return (-1);
	if (all_data->t_t_eat == -1 || all_data->t_t_sleep == -1
		|| all_data->meals_to_eat == -1)
		return (-1);
	if (all_data->n_philo == 0 || all_data->n_philo > 200)
		return (-1);
	return (0);
}

void	init_everything(t_all_data *all_data)
{
	int i;
	char *s;
	char *s_n;

	sem_unlink("/forks_sem");
	sem_unlink("/printing_sem");
	sem_unlink("/meals_eaten_sem");
	all_data->forks_sem = sem_open("/forks_sem", O_CREAT, 0777,
			all_data->n_philo);
	all_data->printing_sem = sem_open("/printing_sem", O_CREAT, 0777, 1);
	all_data->meals_eaten_sem = sem_open("/meals_eaten_sem", O_CREAT, 0777, 0);
	i = 0;
	while (i < all_data->n_philo)
	{
		s_n = ft_itoa(i + 1);
		s = ft_strjoin("/last_meal_sem", s_n);
		sem_unlink(s);
		all_data->philos[i].last_meal_sem = sem_open(s, O_CREAT, 0777, 1);
		free(s_n);
		free(s);
		all_data->philos[i].index = i;
		all_data->philos[i].meals_eaten = 0;
		all_data->philos[i].all_data = all_data;
		i++;
	}
}
