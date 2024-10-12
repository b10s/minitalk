/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aenshin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 21:49:56 by aenshin           #+#    #+#             */
/*   Updated: 2024/10/12 21:49:57 by aenshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>

static	short	int	ft_detect_sign(int n);
static	short	int	ft_count_future_len(int n);
size_t			ft_strlen(const char *s);

void	err(char *err_msg)
{
	write(1, err_msg, ft_strlen(err_msg));
	exit(1);
}

size_t
	ft_strlen(const char *s) {
	size_t	res;

	res = 0;
	while (*s)
	{
		res++;
		s++;
	}
	return (res);
}

char *
	ft_itoa(int n) {
	unsigned long	x;
	short int		len;
	short int		i;
	short int		sign;
	char			*res;

	res = NULL;
	sign = ft_detect_sign(n);
	len = ft_count_future_len(n) + sign;
	res = malloc(len + 1);
	if (res == NULL)
		return (NULL);
	x = n;
	if (sign == 1)
		x = -x ;
	res[0] = '-';
	i = 0;
	while (++i <= len - sign)
	{
		res[len - i] = '0' + x % 10;
		x = x / 10;
	}
	res[len] = '\0';
	return (res);
}

static short int
	ft_detect_sign(int n) {
	if (n < 0)
		return (1);
	return (0);
}

static short int
	ft_count_future_len(int n) {
	short int	res;

	if (n == 0)
		return (1);
	res = 0;
	while (n != 0)
	{
		res++;
		n = n / 10;
	}
	return (res);
}
