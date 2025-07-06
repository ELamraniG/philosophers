/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 17:51:28 by moel-amr          #+#    #+#             */
/*   Updated: 2025/07/06 18:08:25 by moel-amr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "threads.h"

long	ft_get_time(void)
{
	struct timeval	tm;

	gettimeofday(&tm, NULL);
	return (tm.tv_sec * 1000 + tm.tv_usec / 1000);
}

void	printing_stuff(t_philo *philo, char *s)
{
	sem_wait(philo->all_data->printing_sem);
	printf("%lu %d %s\n", ft_get_time() - (philo->all_data->start_time),
		philo->index + 1, s);
	sem_post(philo->all_data->printing_sem);
}