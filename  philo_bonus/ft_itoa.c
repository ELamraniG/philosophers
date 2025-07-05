/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moel-amr <moel-amr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 17:40:36 by moel-amr          #+#    #+#             */
/*   Updated: 2025/07/05 17:17:51 by moel-amr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "threads.h"

static void	ft_put_the_damn_number_r(int n, char *s, int len)
{
	int	i;

	i = 0;
	if (n < 0)
	{
		n *= -1;
		i++;
		len++;
	}
	len--;
	while (len >= i)
	{
		s[len] = (n % 10) + 48;
		n /= 10;
		len--;
	}
}

static int	ft_number_len(int n)
{
	int	i;

	i = 0;
	if (n < 0)
		n *= -1;
	while (n)
	{
		i++;
		n = n / 10;
	}
	return (i);
}

static char	*ft_simplify(int n)
{
	char	*s;

	s = NULL;
	if (n == 0)
	{
		s = malloc (sizeof(char) * 2);
		if (!s)
			return (NULL);
		ft_strlcpy(s, "0", 2);
	}
	else if (n == -2147483648)
	{
		s = malloc (sizeof(char) * 12);
		if (!s)
			return (NULL);
		ft_strlcpy(s, "-2147483648", 12);
	}
	return (s);
}

char	*ft_itoa(int n)
{
	char	*s;

	s = NULL;
	if (n == 0 || n == -2147483648)
		s = ft_simplify(n);
	else
	{
		if (n < 0)
		{
			s = malloc (sizeof(char) * (ft_number_len(n) + 2));
			if (!s)
				return (NULL);
			s[0] = '-';
			s[ft_number_len(n) + 1] = 0;
		}
		else if (n > 0)
		{
			s = malloc (sizeof(char) * (ft_number_len(n) + 1));
			if (!s)
				return (NULL);
			s[ft_number_len(n)] = 0;
		}
		ft_put_the_damn_number_r(n, s, ft_number_len(n));
	}
	return (s);
}