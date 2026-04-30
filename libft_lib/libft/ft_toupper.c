/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:56:25 by nildruon          #+#    #+#             */
/*   Updated: 2025/10/18 14:55:08 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}

/*
#include <stdio.h>
int	main(void)
{
	printf("%c\n", ft_toupper('c'));
	printf("%c\n", ft_toupper('N'));
	printf("%c\n", ft_toupper('a'));
	printf("%c\n", ft_toupper('1'));
}*/