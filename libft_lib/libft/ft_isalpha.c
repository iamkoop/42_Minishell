/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 14:29:16 by nildruon          #+#    #+#             */
/*   Updated: 2025/10/17 18:16:44 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

/*
#include <stdio.h>
int	main(void)
{
	printf("%d\n", ft_isalpha(3));
	printf("%d\n", ft_isalpha('a'));
	printf("%d\n", ft_isalpha(120));
}*/
