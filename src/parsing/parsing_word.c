/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 12:20:37 by bastalze          #+#    #+#             */
/*   Updated: 2026/08/20 21:34:39 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int			add_word_to_struct(t_cmd_data *cmd_data,
				t_minishell *mini);
static int	transfer_existing_word(t_command *curr_cmd, char **tmp_argv,
				t_word_iteri *w);
static int	transfer_new_word(char **tmp_argv, t_word_iteri *w,
				char **word);

int	add_word_to_struct(t_cmd_data *cmd_data,
		t_minishell *mini)
{
	t_word_iteri	w;
	char			**tmp_argv;
	t_command		*curr_cmd;
	char			**word;

	word = (char **)&mini->arena_split_tokens.data;
	if (!mini->quote_mode_entered && word[0][0] == 0)
		return (0);
	ft_bzero(&w, sizeof(t_word_iteri));
	curr_cmd = (t_command *)cmd_data->tail->content;
	tmp_argv = ft_calloc(ft_2darraylen(word) + ft_strarraylen(curr_cmd->argv)
		+ 1, sizeof(char *));
	if (!tmp_argv)
		return (perror("minishell: malloc failed"), mini->exit_status = 1, 1);
	if (transfer_existing_word(curr_cmd, tmp_argv, &w))
		return (mini->exit_status = 1, 1);
	if (transfer_new_word(tmp_argv, &w, word))
		return (mini->exit_status = 1, 1);
	tmp_argv[w.argv_i + w.i] = NULL;
	free_strarray(curr_cmd->argv);
	curr_cmd->argv = tmp_argv;
	mini->quote_mode_entered = false;
	return (0);
}

static int	transfer_new_word(char **tmp_argv, t_word_iteri *w,
				char **word)
{
	w->i = 0;
	while (word[w->i])
	{
		tmp_argv[w->argv_i + w->i] = ft_calloc(ft_strlen(word[w->i]) + 1, 1);
		if (!tmp_argv[w->argv_i + w->i])
			return (perror("minishell: malloc failed"), 1);
		w->j = 0;
		while (word[w->i][w->j])
		{
			tmp_argv[w->argv_i + w->i][w->j] = word[w->i][w->j];
			w->j++;
		}
		tmp_argv[w->argv_i + w->i][w->j] = 0;
		w->i++;
	}
	return (0);
}

static int	transfer_existing_word(t_command *curr_cmd, char **tmp_argv,
				t_word_iteri *w)
{
	w->argv_i = 0;
	while (curr_cmd->argv && curr_cmd->argv[w->argv_i])
	{
		tmp_argv[w->argv_i] = ft_calloc(ft_strlen(curr_cmd->argv[w->argv_i])
					+ 1, 1);
		if (!tmp_argv[w->argv_i])
		{
			perror("minishell: malloc failed");
			free_strarray(tmp_argv);
			return (1);
		}
		w->argv_j = 0;
		while (curr_cmd->argv[w->argv_i][w->argv_j])
		{
			tmp_argv[w->argv_i][w->argv_j]
				= curr_cmd->argv[w->argv_i][w->argv_j];
			(w->argv_j)++;
		}
		tmp_argv[w->argv_i][w->argv_j] = 0;
		(w->argv_i)++;
	}
	return (0);
}
