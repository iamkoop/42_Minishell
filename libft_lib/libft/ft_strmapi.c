/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 16:23:17 by nildruon          #+#    #+#             */
/*   Updated: 2025/10/16 21:33:09 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	char	*str;

	i = 0;
	str = (char *)malloc(ft_strlen(s) + 1);
	if (!s || !f)
		return (NULL);
	while (s[i])
	{
		str[i] = f(i, s[i]);
		i++;
	}
	str[i] = '\0';
	return (str);
}

/*
#include <stdio.h>

char is_even(unsigned int num, char c )
{
	if (num == 0)
	{
		c -= 32;
		return (c);
	}
	if ((num % 2 == 0) && c != 32)
		c -= 32;
	return (c);
} 

int main(void)
{
	char s1[] = "cats and dogs";
	char *result = ft_strmapi(s1, is_even);
	if (result)
	{
		printf("%s\n", result);
		free(result);
	}
}*/