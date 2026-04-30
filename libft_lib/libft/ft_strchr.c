/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 11:57:38 by nildruon          #+#    #+#             */
/*   Updated: 2025/10/18 14:50:58 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//casted the c cuz of edgecase 't' +256
char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
		{
			return ((char *)&s[i]);
		}
		i++;
	}
	if (c == '\0')
		return ((char *)&s[i]);
	return (0);
}

/*
#include <stdio.h>
#include <bsd/string.h>
int	main(void)
{
	printf("%s\n", ft_strchr("Hello worldt", 't' + 256));
	printf("%s\n", strchr("Hello worldt", 't' + 256));
}*/
