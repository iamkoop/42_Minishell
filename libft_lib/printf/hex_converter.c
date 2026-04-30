/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hex_converter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 16:19:35 by nildruon          #+#    #+#             */
/*   Updated: 2026/02/01 21:06:03 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	print_hex_help(int upper, unsigned int nb)
{
	if (upper)
		ft_putchar_fd('F' - (15 - nb), 1);
	else
		ft_putchar_fd('f' - (15 - nb), 1);
}

int	print_hex(char c, uintptr_t nb, int len)
{
	int			upper;
	uintptr_t	num;

	upper = 1;
	if (c == 'x')
		upper = 0;
	if (nb <= 15)
	{
		if (nb < 10)
			ft_putchar_fd('0' + nb, 1);
		else
			print_hex_help(upper, nb);
		return (len + 1);
	}
	if (nb >= 16)
		len = print_hex(c, nb / 16, len);
	num = (nb % 16);
	if (num < 10)
		ft_putchar_fd('0' + num, 1);
	else
		print_hex_help(upper, num);
	return (len + 1);
}

/* #include <stdio.h>

int main (void)
{
	int i;
	i = print_hex('x', 12345678910, 0);
	printf("\n");
	printf("%d", i);
} */