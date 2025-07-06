/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 17:51:28 by moel-amr          #+#    #+#             */
/*   Updated: 2025/07/06 18:06:30 by moel-amr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "threads.h"

long	ft_get_time(void)
{
	struct timeval	tm;

	gettimeofday(&tm, NULL);
	return (tm.tv_sec * 1000 + tm.tv_usec / 1000);
}
void	do_one_philo(t_all_data *all_data)
{
	int	pid;

	pid = fork();
	if (pid == 0)
	{
		printf("%lu 1 is thinking\n", ft_get_time() - all_data->start_time);
		printf("%lu 1 is has taken a fork\n", ft_get_time()
			- all_data->start_time);
		usleep(all_data->t_t_die * 1000);
		printf("%lu 1 died\n", ft_get_time() - all_data->start_time);
	}
	// wait(pi)
	exit(0);
}

void	printing_stuff(t_philo *philo, char *s)
{
	sem_wait(philo->all_data->printing_sem);
	printf("%lu %d %s\n", ft_get_time() - (philo->all_data->start_time),
		philo->index + 1, s);
	sem_post(philo->all_data->printing_sem);
}