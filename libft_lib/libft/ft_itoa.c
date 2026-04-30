/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 11:46:57 by nildruon          #+#    #+#             */
/*   Updated: 2025/10/16 21:40:51 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_l_of_int(int n)
{
	int		len;

	len = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		n *= -1;
	while (n > 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static char	*converter_help(char *asc, int n, int i)
{
	int		mod;
	char	c;

	while (n >= 0)
	{
		mod = n;
		mod %= 10;
		c = mod + '0';
		asc[i] = c;
		if (n < 10)
			break ;
		n /= 10;
		i--;
	}
	return (asc);
}

static char	*converter(char *asc, int n)
{
	int		i;

	i = 0;
	if (n < 0)
	{
		n *= -1;
		asc[i] = '-';
		i++;
	}
	i += get_l_of_int(n);
	asc[i] = '\0';
	i--;
	asc = converter_help(asc, n, i);
	return (asc);
}

char	*ft_itoa(int n)
{
	int		len;
	char	*asc;

	len = get_l_of_int(n);
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n < 0)
		asc = (char *)malloc(len + 2);
	else
		asc = (char *)malloc(len + 1);
	if (!asc)
		return (NULL);
	asc = converter(asc, n);
	return (asc);
}

/*
#include <stdio.h>
int	main(void)
{
	printf("%s\n", ft_itoa(-1));
	printf("%s\n", ft_itoa(124124));
}*/
