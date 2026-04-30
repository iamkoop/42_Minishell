/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 13:27:07 by nildruon          #+#    #+#             */
/*   Updated: 2025/10/16 21:21:00 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isprint(int c)
{
	if (c < 32 || c > 126)
		return (0);
	return (1);
}

/*
#include <stdio.h>
int	main(void)
{
	printf("%d\n", ft_isprint(3));
	printf("%d\n", ft_isprint(' '));
	printf("%d\n", ft_isprint('a'));
	printf("%d\n", ft_isprint(120));
}*/
