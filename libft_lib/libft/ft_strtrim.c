/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 13:00:10 by nildruon          #+#    #+#             */
/*   Updated: 2025/10/16 21:50:05 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*trimmed;
	int		end;
	int		start;
	int		i;

	end = ft_strlen(s1);
	start = 0;
	i = 0;
	while (ft_strchr(set, s1[start]) != NULL && s1[start])
		start++;
	end--;
	while (ft_strchr(set, s1[end]) != NULL && end > start)
		end--;
	trimmed = (char *)malloc((end - start + 1) + 1);
	if (!trimmed)
		return (NULL);
	while (start <= end)
	{
		trimmed[i] = s1[start];
		i++;
		start++;
	}
	trimmed[i] = '\0';
	return (trimmed);
}

/*
#include <stdio.h>
int main(void)
{
	const char *s1 = "Hello my Name is tobi";
	const char *set = "iH";
	printf("%s\n", s1);
	printf("%s\n", set);
	printf("%s\n", ft_strtrim(s1,set));
}*/
