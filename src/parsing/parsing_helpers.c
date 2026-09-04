/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 12:13:54 by bastalze          #+#    #+#             */
/*   Updated: 2026/08/20 21:34:31 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int		is_redirection(t_arena *arena_tokens, t_token_iteri *iteri);
void	free_strarray(char **array);
size_t	ft_2darraylen(char **word);
size_t	ft_strarraylen(char **argv);

int	is_redirection(t_arena *arena_tokens, t_token_iteri *iteri)
{
	if (arena_tokens->pos != 0
		&& ((iteri->tok - 1)->token_type == REDIR_IN
			|| (iteri->tok - 1)->token_type == REDIR_OUT
			|| (iteri->tok - 1)->token_type == REDIR_OUT_A
			|| (iteri->tok - 1)->token_type == HERE_DOC)
		&& iteri->tok->token_type == WORD)
		return (1);
	return (0);
}

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

size_t	ft_2darraylen(char **word)
{
	size_t	i;

	i = 0;
	while (word[i])
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
