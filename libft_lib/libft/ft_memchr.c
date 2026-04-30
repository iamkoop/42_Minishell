/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 15:53:34 by nildruon          #+#    #+#             */
/*   Updated: 2026/02/04 15:08:08 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*m;
	unsigned char		ch;
	size_t				i;

	m = (unsigned char *)s;
	ch = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (m[i] == ch)
		{
			return ((void *)&m[i]);
		}
		i++;
	}
	return (NULL);
}

/*
#include <stdio.h>
int main(void)
{
	unsigned char str1[] = "Hubbabubba";
	unsigned char str2[] = "Hubbabubba";

	printf("%s\n", (unsigned char *)ft_memchr(str1, 'a', 10));
	printf("%s\n", (unsigned char *)ft_memchr(str2, 'c', 100));
}*/
