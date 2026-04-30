/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 11:57:31 by nildruon          #+#    #+#             */
/*   Updated: 2026/04/21 20:45:44 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	cnt_words(char const *s, char c)
{
	int	i;
	int	cnt;

	i = 0;
	cnt = 0;
	while (s[i])
	{
		if (s[i] != c && ((s[i + 1] == c) || s[i + 1] == '\0'))
			cnt++;
		i++;
	}
	return (cnt);
}

static int	word_length(char const *s, char del)
{
	int	cnt;

	cnt = 0;
	while (s[cnt] && s[cnt] != del)
		cnt++;
	return (cnt);
}

static void	*freeup(char **s, int i)
{
	while (--i >= 0)
		free(s[i]);
	free(s);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**splitted;
	int		i;
	int		j;

	i = 0;
	if (!s)
		return (NULL);
	splitted = (char **)calloc(sizeof(char *), (cnt_words(s, c) + 1));
	if (!splitted)
		return (NULL);
	j = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			splitted[j] = ft_substr(s, i, word_length(&s[i], c));
			if (!splitted[j])
				return (freeup(splitted, j));
			j++;
			i += word_length(&s[i], c);
		}
		else
			i++;
	}
	return (splitted[j] = NULL, splitted);
}

/*
#include <stdio.h>
int	main(void)
{
	char **res = ft_split("Hello my Name is joe ", ' ');
	int i = 0;

	if (!res)
		return (1);
	while (res[i])
	{
		printf("%s\n", res[i]);
		i++;
	}
} */