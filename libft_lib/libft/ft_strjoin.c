/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 11:59:29 by nildruon          #+#    #+#             */
/*   Updated: 2025/10/18 14:46:53 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*concat;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	concat = (char *)malloc(s1_len + s2_len + 1);
	if (!concat)
		return (NULL);
	ft_strlcpy(concat, s1, s1_len + 1);
	ft_strlcat(concat, s2, s2_len + s1_len + 1);
	return (concat);
}

/*
#include <stdio.h>
int main(void)
{
	const char *s1 = "Den Polizist nennt";
	const char *s2 = " ma a Kiwara";
	printf("%s\n", s1);
	printf("%s\n", s2);
	printf("%s\n", ft_strjoin(s1,s2));
}*/