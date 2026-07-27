/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_reading+writing.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 12:24:34 by bastalze          #+#    #+#             */
/*   Updated: 2026/07/27 12:24:37 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../minishell.h"

int			adding_heredoc_into_file(int fd, bool expansion, char *delimiter,
				t_single_linked_node *env);
static int	var_expansion(char **heredoc_input,
				char word[WORD_AMOUNT][WORD_STR_SIZE],
				t_single_linked_node *env);
static int	write_heredoc_line(char *heredoc_input, int fd);
static int	write_check(ssize_t c_written, size_t len);

int	adding_heredoc_into_file(int fd, bool expansion, char *delimiter,
				t_single_linked_node *env)
{
	char	*heredoc_input;
	char	word[WORD_AMOUNT][WORD_STR_SIZE];

	ft_bzero(word, WORD_AMOUNT * WORD_STR_SIZE);
	while (42)
	{
		heredoc_input = readline("> ");
//		if (!heredoc_input)
//			return (1);
		if (!ft_strncmp(delimiter, heredoc_input, HD_DELIMITER_LEN))
			return (free(heredoc_input), 0);
		if (expansion)
		{
			if (var_expansion(&heredoc_input, word, env))
				return (free(heredoc_input), 1);
		}
		if (write_heredoc_line(heredoc_input, fd))
			return (free(heredoc_input), 1);
		free(heredoc_input);
	}
	return (0);
}

static int	var_expansion(char **heredoc_input,
				char word[WORD_AMOUNT][WORD_STR_SIZE],
				t_single_linked_node *env)
{
	char	*tmp_heredoc_input;

	if (quote_rm_var_expan(*heredoc_input, word, env, true))
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
	ssize_t	c_written;

	c_written = write(fd, heredoc_input, ft_strlen(heredoc_input));
	if (write_check(c_written, ft_strlen(heredoc_input)))
		return (1);
	c_written = write(fd, "\n", 1);
	if (write_check(c_written, 1))
		return (1);
	return (0);
}

static int	write_check(ssize_t c_written, size_t len)
{
	if (c_written == -1)
	{
		perror("minishell: write failure in heredoc");
		return (1);
	}
	else if (c_written != (ssize_t)len)
	{
		error("cannot create temp file for here-document: No space left \
				on device");
		return (1);
	}
	return (0);
}
