/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 16:47:06 by nildruon          #+#    #+#             */
/*   Updated: 2025/10/16 21:23:18 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*str1;
	unsigned char	*str2;
	size_t			i;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	i = 0;
	if (n == 0)
		return (0);
	while (i < n - 1 && str1[i] == str2[i])
	{
		i++;
	}
	return (str1[i] - str2[i]);
}

/*
#include <stdio.h>
int	main(void)
{
	
	char s2[] = {0, 0, 127, 0};
	char s3[] = {0, 0, 42, 0};
	printf("%d\n", ft_memcmp(s2, s3, 4));
	printf("%d\n", ft_memcmp("Perry the platipus", "Perry the platipus", 18));
	printf("%d\n", ft_memcmp("abc", "abd", 3));
	printf("%d\n", ft_memcmp("abc", "abd", 2));
	printf("%d\n", ft_memcmp("101", "505", 3));
	printf("%d\n", ft_memcmp("101", "105", 3));
	printf("%d\n", ft_memcmp("42", "42", 1));
	printf("%d\n", ft_memcmp("4", "4", 1));
	printf("%d\n", ft_memcmp("4", "5", 1));
	printf("%d\n", ft_memcmp("4", "5", 0));
}*/
