/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 11:31:05 by nildruon          #+#    #+#             */
/*   Updated: 2025/10/16 21:34:33 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	littlelen;

	i = 0;
	littlelen = ft_strlen(little);
	if (ft_strlen(little) == 0)
		return ((char *)big);
	while (big[i] && (littlelen + i <= len))
	{
		if (ft_strncmp(&big[i], little, littlelen) == 0)
			return ((char *)&big[i]);
		i++;
	}
	return (NULL);
}

/*
#include <stdio.h>
int	main()
{
	printf("%s\n", ft_strnstr("Foo Bar Baz", "Bar", 4));
	printf("%s\n", ft_strnstr("Foo Bar Baz", "Bar", 12));
	printf("%s\n", ft_strnstr("Foo Bar Baz", "\0", 8));
	printf("%s\n", ft_strnstr("Foo Bar Baz", "Biz", 8));
	printf("%s\n", ft_strnstr("Foo Baz Baz", "", 8));
	printf("%s\n", ft_strnstr("Foo Baz Baz", "Foo", 8));
}*/