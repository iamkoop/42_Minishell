/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 11:17:02 by bastalze          #+#    #+#             */
/*   Updated: 2026/08/20 21:34:28 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int			here_doc(char *input, t_single_linked_node *env,
				t_minishell *mini, t_token_iteri *iteri);
static int	prepare_delimiter(char *delimiter, t_token_iteri *iteri,
				bool *expansion);
static int	heredoc_filename_creation(char *filename, char *input,
				t_token_iteri *iteri);
static int	creating_read_fd(char *filename, t_token_iteri *iteri);

int	here_doc(char *input, t_single_linked_node *env, t_minishell *mini,
		t_token_iteri *iteri)
{
	char	delimiter[HD_DELIMITER_LEN];
	char	filename[16];
	bool	expansion;

	expansion = false;
	ft_bzero(delimiter, sizeof(char) * HD_DELIMITER_LEN);
	if (prepare_delimiter(delimiter, iteri, &expansion))
		return (1);
	if (heredoc_filename_creation(filename, input, iteri))
		return (1);
	mini->heredoc_write_fd = open((const char *) filename,
		O_WRONLY | O_CLOEXEC | O_EXCL | O_CREAT, 0600);
	if (mini->heredoc_write_fd == -1)
		return (perror(NULL), 1);
	if (adding_heredoc_into_file(mini, expansion, delimiter, env))
		return (unlink(filename), close(mini->heredoc_write_fd), 1);
	if (creating_read_fd(filename, iteri))
		return (close(mini->heredoc_write_fd), 1);
	close(mini->heredoc_write_fd);
	unlink(filename);
	return (0);
}

//There is a separate read fd because the file offset of the write fd would be
//where the last thing is written and then read would continue reading there.
//This way with the read fd you start reading from the beginning of the file.

static int	creating_read_fd(char *filename, t_token_iteri *iteri)
{
	int		read_fd;
	char	*fd_str;

	read_fd = open((const char *) filename, O_RDONLY);
	if (read_fd == -1)
		return (unlink(filename), perror(NULL), 1);
	fd_str = ft_itoa(read_fd);
	if (!fd_str)
		return (close(read_fd), unlink(filename), perror(NULL), 1);
	ft_strlcpy((iteri->tok - 1)->token_str, fd_str, TOKEN_STR_SIZE);
	free(fd_str);
	return (0);
}

// read_fd is stored in token_str from the word token_type 
// (that contained the delimiter) as a string

static int	prepare_delimiter(char *delimiter, t_token_iteri *iteri,
				bool *expansion)
{
	size_t	len;

	len = ft_strlen((iteri->tok - 1)->token_str);
	if (len >= HD_DELIMITER_LEN)
	{
		error("exceeding memory limit: Heredoc delimiter \
				\nRaise HD_DELIMITER_LEN in minishell.h");
		return (1);
	}
	ft_strlcpy(delimiter, (iteri->tok - 1)->token_str, len +1);
	if (strchr(delimiter, '\"') || strchr(delimiter, '\''))
		quote_removal(delimiter);
	else
		*expansion = true;
	return (0);
}

static int	heredoc_filename_creation(char *filename, char *input,
				t_token_iteri *iteri)
{
	static int	num;
	char		*start;

	start = ".hd/File_";
	memcpy(filename, start, 9);
	if (num > 99)
	{
		error("limit of amount of heredocs reached");
		return (1);
	}
	filename[9] = num / 10 + '0';
	filename[10] = num % 10 + '0';
	filename[11] = '.';
	filename[12] = 't';
	filename[13] = 'x';
	filename[14] = 't';
	filename[15] = 0;
	num++;
	if (input[iteri->i] == 0)
		num = 0;
	return (0);
}
