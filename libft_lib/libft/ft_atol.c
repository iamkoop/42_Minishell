/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 17:28:40 by nildruon          #+#    #+#             */
/*   Updated: 2026/02/04 14:44:27 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

long	ft_atol(const char *nptr)
{
	long	i;
	long	sign;
	long	num;

	i = 0;
	sign = 1;
	num = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == ' ')
		i++;
	if (nptr[i + 1] == '+' || nptr[i + 1] == '-')
		return (0);
	if (nptr[i] == '+')
		i++;
	if (nptr[i] == '-')
	{
		sign *= -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		num = (num * 10) + (nptr[i] - '0');
		i++;
	}
	i = num * sign;
	return (i);
}

/* #include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	printf("ft_atol: %ld\n", ft_atol("2147483650"));
	printf("atol: %ld\n", atol("2147483650"));
} */