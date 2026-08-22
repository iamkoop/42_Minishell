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

int			adding_heredoc_into_file(t_minishell *mini, bool expansion, char *delimiter,
				t_single_linked_node *env);
static int	var_expansion(char **heredoc_input,
				char word[WORD_AMOUNT][WORD_STR_SIZE],
				t_single_linked_node *env);
static int	write_heredoc_line(char *heredoc_input, int fd);
int			ft_write(int fd, char *line, size_t len);

int	adding_heredoc_into_file(t_minishell *mini, bool expansion, char *delimiter,
				t_single_linked_node *env)
{
	char	*heredoc_input;
	char	word[WORD_AMOUNT][WORD_STR_SIZE];

	ft_bzero(word, WORD_AMOUNT * WORD_STR_SIZE);
	while (42)
	{
		heredoc_input = readline("> ");
		if (g_signal == SIGINT)
		{
			mini->exit_status = 130;
			g_signal = 0;
            free(heredoc_input);
			return (1);
		}
		if (!heredoc_input)
			return (error("warning: here-document delimited by end-of-file "
						"instead of delimiter"), 0);
		if (!ft_strncmp(delimiter, heredoc_input, HD_DELIMITER_LEN))
			return (free(heredoc_input), 0);
		if (expansion)
		{
			if (var_expansion(&heredoc_input, word, env))
				return (free(heredoc_input), 1);
		}
		if (write_heredoc_line(heredoc_input, mini->heredoc_write_fd))
			return (free(heredoc_input), 1);
		free(heredoc_input);
	}
	return (0);
}

static int	var_expansion(char **heredoc_input,
				char word[WORD_AMOUNT][WORD_STR_SIZE],
				t_single_linked_node *env)
{
	char		*tmp_heredoc_input;
	t_quote_iteri   exv;

	ft_bzero(&exv, sizeof(t_quote_iteri));
	exv.heredoc = true;
	if (quote_rm_var_expan(*heredoc_input, word, env, &exv))
		return (1);
	assert(word[1][0] == 0);
	tmp_heredoc_input = calloc(1, ft_strlen(word[0]) + 1);
	if (!tmp_heredoc_input)
		return (1);
	ft_strlcpy(tmp_heredoc_input, word[0], ft_strlen(word[0]) + 1);
	free(*heredoc_input);
	*heredoc_input = tmp_heredoc_input;
	return (0);
}

static int	write_heredoc_line(char *heredoc_input, int fd)
{
	
	if (ft_write(fd, heredoc_input, ft_strlen(heredoc_input)))
		return (1);
	if (ft_write(fd, "\n", 1))
		return (1);
	return (0);
}

int	ft_write(int fd, char *line, size_t len)
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
