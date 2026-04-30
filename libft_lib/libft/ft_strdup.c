/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 17:15:47 by nildruon          #+#    #+#             */
/*   Updated: 2025/10/16 21:39:18 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*cpy;
	size_t	i;

	len = ft_strlen(s);
	cpy = (char *)malloc(len + 1);
	if (!cpy)
		return (NULL);
	i = 0;
	while (i <= len)
	{
		cpy[i] = s[i];
		i++;
	}
	return (cpy);
}

/*
#include <stdio.h>
int main(void)
{
	const char *s = "Kiwara";
	printf("%s\n", s);
	printf("%s\n", ft_strdup(s));
}*/