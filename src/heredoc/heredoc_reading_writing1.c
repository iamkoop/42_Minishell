/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_reading+writing1.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 12:24:34 by bastalze          #+#    #+#             */
/*   Updated: 2026/08/20 21:34:22 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int			adding_heredoc_into_file(t_minishell *mini, bool expansion,
				char *delimiter, t_single_linked_node *env);
static int	process_heredoc_line(t_minishell *mini, t_heredoc_data *hd_data,
				char *delimiter, t_single_linked_node *env);
static int	sigint_check(t_minishell *mini, t_heredoc_data *hd_data);
static int	input_handling_after_eof(t_heredoc_data *hd_data,
				t_minishell *mini);
static int	merging_last_eof_input_with_heredoc_input(t_heredoc_data *hd_data,
				t_minishell *mini);

int	adding_heredoc_into_file(t_minishell *mini, bool expansion, char *delimiter,
				t_single_linked_node *env)
{
	t_heredoc_data	hd_data;
	int				status;

	ft_bzero(&hd_data, sizeof(hd_data));
	hd_data.expansion = expansion;
	while (42)
	{
		if (isatty(STDIN_FILENO) && hd_data.eof_nonempty_line == false)
			write(STDERR_FILENO, "> ", 2);
		hd_data.heredoc_input = get_next_line(STDIN_FILENO);
		status = process_heredoc_line(mini, &hd_data, delimiter, env);
		if (status == 1)
			return (1);
		if (status == 2)
			return (0);
	}
	return (0);
}

static int	process_heredoc_line(t_minishell *mini, t_heredoc_data *hd_data,
				char *delimiter, t_single_linked_node *env)
{
	if (sigint_check(mini, hd_data))
		return (1);
	if (!hd_data->heredoc_input && hd_data->eof_nonempty_line == false)
		return (write(2, "\nminishell: warning: here-document delimited by"
				" end-of-file instead of delimiter\n", 81), 2);
	if (!hd_data->heredoc_input && hd_data->eof_nonempty_line == true)
		return (0);
	if (hd_data->heredoc_input[ft_strlen(hd_data->heredoc_input) - 1] != '\n')
	{
		if (input_handling_after_eof(hd_data, mini))
			return (1);
		return (0);
	}
	if (merging_last_eof_input_with_heredoc_input(hd_data, mini))
		return (1);
	if (!ft_strcmp(delimiter, hd_data->heredoc_input))
		return (free(hd_data->heredoc_input), 2);
	if (expand_n_write(hd_data, mini, env))
		return (1);
	return (0);
}

static int	sigint_check(t_minishell *mini, t_heredoc_data *hd_data)
{
	if (g_signal == SIGINT)
	{
		mini->exit_status = 130;
		g_signal = 0;
		write(STDERR_FILENO, "\n", 1);
		free(hd_data->heredoc_input);
		free(hd_data->eof_input);
		return (1);
	}
	return (0);
}

static int	input_handling_after_eof(t_heredoc_data *hd_data, t_minishell *mini)
{
	char	*tmp;

	if (hd_data->eof_input)
	{
		tmp = hd_data->eof_input;
		hd_data->eof_input = ft_strjoin(tmp, hd_data->heredoc_input);
		free(tmp);
		free(hd_data->heredoc_input);
		if (!hd_data->eof_input)
			return (mini->exit_status = 1,
				perror("minishell: malloc failure"), 1);
		hd_data->heredoc_input = NULL;
	}
	else
	{
		hd_data->eof_input = hd_data->heredoc_input;
		hd_data->heredoc_input = NULL;
	}
	hd_data->eof_nonempty_line = true;
	return (0);
}

static int	merging_last_eof_input_with_heredoc_input(t_heredoc_data *hd_data,
				t_minishell *mini)
{
	char	*tmp;

	if (hd_data->eof_input)
	{
		tmp = hd_data->heredoc_input;
		hd_data->heredoc_input = ft_strjoin(hd_data->eof_input, tmp);
		free(tmp);
		free(hd_data->eof_input);
		if (!hd_data->heredoc_input)
			return (mini->exit_status = 1,
				perror("minishell: malloc failure"), 1);
		hd_data->eof_input = NULL;
		hd_data->eof_nonempty_line = false;
	}
	return (0);
}
