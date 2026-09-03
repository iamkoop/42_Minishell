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
				t_minishell *mini,
				t_single_linked_node *env);
static int	write_heredoc_line(char *heredoc_input, int fd);
int			ft_write(int fd, char *line, size_t len);

int	adding_heredoc_into_file(t_minishell *mini, bool expansion, char *delimiter,
				t_single_linked_node *env)
{
	char	*heredoc_input;
	char	*eof_input;
	char	*tmp;
	bool	eof_nonempty_line;

	eof_nonempty_line = false;
	eof_input = NULL;
	while (42)
	{
		if(isatty(STDIN_FILENO) && eof_nonempty_line == false)
			write(STDERR_FILENO, "> ", 2);
		heredoc_input = get_next_line(STDIN_FILENO);
		if (g_signal == SIGINT)
		{
			mini->exit_status = 130;
			g_signal = 0;
			write(1, "\n", 1);
            free(heredoc_input);
			free(eof_input);
			return (1);
		}
		if (!heredoc_input && eof_nonempty_line == false)
			return (write(2, "\nminishell: warning: here-document delimited by end-of-file "
						"instead of delimiter\n", 81), 0);
		if (!heredoc_input && eof_nonempty_line == true)
				continue ;
		if (heredoc_input[ft_strlen(heredoc_input) - 1] != '\n')
		{
			if (eof_input)
			{
				tmp = eof_input;
				eof_input = NULL;
				eof_input = ft_strjoin(tmp, heredoc_input);
				if (!eof_input)
				{
					return (mini->exit_status = 1, perror("minishell: malloc failure"),
						free(tmp), free(heredoc_input), 1);
				}
				free(tmp);
				free(heredoc_input);
				heredoc_input = NULL;
			}
			else
			{
				eof_input = heredoc_input;
				heredoc_input = NULL;
			}
			eof_nonempty_line = true;
			continue ;
		}
		if (eof_input)
		{
			tmp = heredoc_input;
			heredoc_input = NULL;
			heredoc_input = ft_strjoin(eof_input, tmp);
			if (!heredoc_input)
				return (mini->exit_status = 1, perror("minishell: malloc failure"),
					free(tmp), free(eof_input), 1);
			free(tmp);
			free(eof_input);
			eof_input = NULL;
			eof_nonempty_line = false;
		}
		if (!ft_strcmp(delimiter, heredoc_input))
			return (free(heredoc_input), 0);
		if (expansion)
		{
			if (var_expansion(&heredoc_input, mini, env))
				return (free(heredoc_input), mini->exit_status = 1, 1);
		}
		if (write_heredoc_line(heredoc_input, mini->heredoc_write_fd))
			return (free(heredoc_input), mini->exit_status = 1, 1);
		free(heredoc_input);
	}
	return (0);
}

static int	var_expansion(char **heredoc_input,
				t_minishell *mini,
				t_single_linked_node *env)
{
	char			*tmp_heredoc_input;
	t_quote_iteri   exv;
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
	//if (ft_write(fd, "\n", 1))
	//	return (1);
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
