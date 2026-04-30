/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 11:10:05 by nildruon          #+#    #+#             */
/*   Updated: 2026/04/21 20:50:54 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*cpy;
	size_t	i;

	if (len == 0)
		return (ft_calloc(1, 1));
	if (start > ft_strlen(s))
		return (ft_calloc(1, 1));
	if (ft_strlen(s) < start + len)
		cpy = (char *)malloc(ft_strlen(s) - start + 1);
	else
		cpy = (char *)malloc(len + 1);
	i = 0;
	if (!cpy)
		return (NULL);
	while (i < len && start + i < ft_strlen(s))
	{
		cpy[i] = s[start + i];
		i++;
	}
	cpy[i] = '\0';
	return (cpy);
}

/*
#include <stdio.h>
int main (void)
{
	const char *s = "Den Polizist nennt ma a Kiwara";
	char * s1 = "tripouille";
	printf("%s\n", s);
	printf("%s\n", ft_substr(s, 4,8));
	printf("%s\n", s1);
	printf("%s\n", ft_substr(s1, 0,42000));
}*/
