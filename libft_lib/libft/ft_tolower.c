/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:09:23 by nildruon          #+#    #+#             */
/*   Updated: 2025/10/16 21:37:05 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	return (c);
}

/*
#include <stdio.h>
int	main(void)
{
	printf("%c\n", ft_tolower('c'));
	printf("%c\n", ft_tolower('q'));
	printf("%c\n", ft_tolower('a'));
	printf("%c\n", ft_tolower('1'));
}*/