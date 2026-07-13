/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 13:58:00 by bastalze          #+#    #+#             */
/*   Updated: 2026/05/26 14:10:30 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../minishell.h"

char		*quote_removal(char *delimiter);
static void	add_char(char *delimiter, int *i, int *j);

char	*quote_removal(char *delimiter)
{
	int		i;
	int		j;
	char	c;

	c = '0';
	i = 0;
	j = 0;
	while (delimiter[i])
	{
		if (delimiter[i] == '\'' || delimiter[i] == '\"')
		{
			c = delimiter[i];
			i++;
			while (delimiter[i] != c)
			{
				delimiter[j] = delimiter[i];
				j++;
				i++;
			}
			i++;
		}
		else
			add_char(delimiter, &i, &j);
	}
	return (delimiter[j] = 0, delimiter);
}

static void	add_char(char *delimiter, int *i, int *j)
{
	delimiter[*j] = delimiter[*i];
	(*j)++;
	(*i)++;
}
