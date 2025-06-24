/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 18:34:39 by moel-amr          #+#    #+#             */
/*   Updated: 2025/01/11 15:18:12 by moel-amr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "threads.h"

static int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

int	free_boring(void)
{
	printf("invalide args\n");
	return (-1);
}

int	ft_atoi(const char *s)
{
	long	res;
	int		i;

	i = 0;
	res = 0;
	if (s[i] == '+')
		i++;
	if (!ft_isdigit(s[i]))
		return (free_boring());
	while (ft_isdigit(s[i]))
	{
		res = ((res * 10) + (s[i] - 48));
		i++;
		if (s[i] != 0 && !ft_isdigit(s[i]))
			return (free_boring());
	}
	if (res > 2147483647 || res < -2147483648)
		return (free_boring());
	return (res);
}
