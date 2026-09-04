/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_reading+writing.c                          :+:      :+:    :+:   */
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
static int	input_handling_after_eof(t_heredoc_data *hd_data,
				t_minishell *mini);
static int	merging_last_eof_input_with_heredoc_input(t_heredoc_data *hd_data,
				t_minishell *mini);
static int	sigint_check(t_minishell *mini, t_heredoc_data *hd_data);
static int	expand_n_write(t_heredoc_data *hd_data,
				t_minishell *mini, t_single_linked_node *env);
static int	var_expansion(char **heredoc_input,
				t_minishell *mini, t_single_linked_node *env);
static int	write_heredoc_line(char *heredoc_input, int fd);
static int	ft_write(int fd, char *line, size_t len);

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

static int	expand_n_write(t_heredoc_data *hd_data,
				t_minishell *mini, t_single_linked_node *env)
{
	if (hd_data->expansion)
	{
		if (var_expansion(&(hd_data->heredoc_input), mini, env))
			return (free(hd_data->heredoc_input), mini->exit_status = 1, 1);
	}
	if (write_heredoc_line(hd_data->heredoc_input, mini->heredoc_write_fd))
		return (free(hd_data->heredoc_input), mini->exit_status = 1, 1);
	free(hd_data->heredoc_input);
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

static int	var_expansion(char **heredoc_input, t_minishell *mini,
				t_single_linked_node *env)
{
	char			*tmp_heredoc_input;
	t_quote_iteri	exv;
	char			**word;

	init_qrve_arena(mini);
	ft_bzero(&exv, sizeof(t_quote_iteri));
	start_first_word(mini, &exv);
	word = (char **)&mini->arena_split_tokens.data;
	exv.heredoc = true;
	if (quote_rm_var_expan(*heredoc_input, mini, env, &exv))
		return (1);
	assert(word[1] == NULL);
	tmp_heredoc_input = ft_calloc(1, ft_strlen(word[0]) + 1);
	if (!tmp_heredoc_input)
		return (perror("minishell: malloc failed"), mini->exit_status = 1, 1);
	ft_strlcpy(tmp_heredoc_input, word[0], ft_strlen(word[0]) + 1);
	free(*heredoc_input);
	*heredoc_input = tmp_heredoc_input;
	return (0);
}

static int	write_heredoc_line(char *heredoc_input, int fd)
{
	if (ft_write(fd, heredoc_input, ft_strlen(heredoc_input)))
		return (1);
	return (0);
}

static int	ft_write(int fd, char *line, size_t len)
{
	ssize_t	c_written;

	c_written = write(fd, line, len);
	if (c_written == -1)
	{
		perror("minishell: write failure");
		return (1);
	}
	else if (c_written != (ssize_t)len)
	{
		error("write failure: No space left on device");
		return (1);
	}
	return (0);
}
