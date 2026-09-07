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

int			expand_n_write(t_heredoc_data *hd_data,
				t_minishell *mini, t_single_linked_node *env);
static int	var_expansion(char **heredoc_input,
				t_minishell *mini, t_single_linked_node *env);
static int	write_heredoc_line(char *heredoc_input, int fd);
static int	ft_write(int fd, char *line, size_t len);

int	expand_n_write(t_heredoc_data *hd_data,
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
