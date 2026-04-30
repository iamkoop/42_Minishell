/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:55:32 by nildruon          #+#    #+#             */
/*   Updated: 2025/10/16 21:20:16 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
		return (1);
	return (0);
}

/*
#include <stdio.h>
int	main(void)
{
	printf("%d\n", ft_isascii(3));
	printf("%d\n", ft_isascii('a'));
	printf("%d\n", ft_isascii(130));
}*/
