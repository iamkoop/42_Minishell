/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 15:20:50 by nildruon          #+#    #+#             */
/*   Updated: 2026/04/16 18:54:17 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (i < n - 1 && s1[i] == s2[i] && s1[i] && s2[i])
	{
		i++;
	}
	return (s1[i] - s2[i]);
}

/*
#include <stdio.h>
int	main(void)
{
	printf("%d\n", ft_strncmp("Perry the platipus", "Perry the platipus", 18));
	printf("%d\n", ft_strncmp("abc", "abd", 3));
	printf("%d\n", ft_strncmp("abc", "abd", 2));
	printf("%d\n", ft_strncmp("101", "505", 3));
	printf("%d\n", ft_strncmp("101", "105", 3));
	printf("%d\n", ft_strncmp("42", "42", 1));
	printf("%d\n", ft_strncmp("4", "4", 1));
	printf("%d\n", ft_strncmp("4", "5", 1));
}*/
