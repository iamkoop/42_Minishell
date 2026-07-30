/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 12:13:54 by bastalze          #+#    #+#             */
/*   Updated: 2026/07/30 12:14:00 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../minishell.h"

void	free_strarray(char **array);
size_t	ft_2darraylen(char word[WORD_AMOUNT][WORD_STR_SIZE]);
size_t	ft_strarraylen(char **argv);

void	free_strarray(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

size_t	ft_2darraylen(char word[WORD_AMOUNT][WORD_STR_SIZE])
{
	size_t	i;

	i = 0;
	while (i < WORD_AMOUNT && word[i][0] != 0)
		i++;
	return (i);
}

size_t	ft_strarraylen(char **argv)
{
	size_t	i;

	if (!argv)
		return (0);
	i = 0;
	while (argv[i])
		i++;
	return (i);
}
