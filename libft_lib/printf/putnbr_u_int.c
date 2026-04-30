/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putnbr_u_int.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 18:09:49 by nildruon          #+#    #+#             */
/*   Updated: 2026/02/01 21:06:16 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	putnbr_u_int(unsigned int n, int len)
{
	char	c;

	if (n >= 10)
	{
		len = putnbr_u_int(n / 10, len);
	}
	c = (n % 10) + '0';
	write(1, &c, 1);
	len++;
	return (len);
}

/* #include <stdio.h>

int main (void)
{
	int i;
	i = putnbr_u_int(123456789, 0);
	printf("\n");
	printf("%d", i);
} */
