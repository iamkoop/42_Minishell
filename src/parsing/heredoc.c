/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 11:17:02 by bastalze          #+#    #+#             */
/*   Updated: 2026/06/03 16:58:18 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../minishell.h"

int	here_doc(char *input, char **env, t_token_node *token_lst,
		t_token_iteri *iteri);
int	prepare_delimiter(char *delimiter, t_token_node *token_lst,
			t_token_iteri *iteri, bool *expansion);
int	heredoc_filename_creation(char *filename, char *input, t_token_iteri *iteri);
int	adding_heredoc_into_file(int fd, bool expansion, char *delimiter);
int write_check(ssize_t c_written, size_t len);

int	here_doc(char *input, char **env, t_token_node *token_lst,
			t_token_iteri *iteri)
{
	char	delimiter[HD_DELIMITER_LEN];
	char	filename[16];
	bool	expansion;
	int		fd;

	env = NULL;	
	expansion = false;
	ft_bzero(delimiter, sizeof(char) * HD_DELIMITER_LEN);
	if (prepare_delimiter(delimiter, token_lst, iteri, &expansion))
		return (1);
	if (heredoc_filename_creation(filename, input, iteri))
		return (1);
	fd = open((const char*)filename, O_TRUNC | O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
	{
		perror(NULL);
		return (1);
	}
	if (adding_heredoc_into_file(fd, expansion, delimiter))
		return (1);
	close(fd);
	ft_strlcpy(token_lst[iteri->token - 1].token_str, filename, 17);
//	tokenization(input, env, token_lst, iteri);
	return (0);
}

int	prepare_delimiter(char *delimiter, t_token_node *token_lst,
			t_token_iteri *iteri, bool *expansion)
{
	size_t	len;

	len = ft_strlen(token_lst[iteri->token - 1].token_str);
	if (len >= HD_DELIMITER_LEN)
	{
		error("exceeding memory limit: Heredoc delimiter \
				\nTo use a longer delimiter raise HD_DELIMITER_LEN \
				in minishell.h");
		return (1);
	}
	ft_strlcpy(delimiter, token_lst[iteri->token - 1].token_str, len +1);
	if (strchr(delimiter, '\"') || strchr(delimiter, '\''))
	{
		*expansion = true;
		quote_removal(delimiter);
	}
	return (0);
}

int	heredoc_filename_creation(char *filename, char *input, t_token_iteri *iteri)
{
	static int	num;
	char		*start;

	start = ".hd/File_";
	memcpy(filename, start, 9);
	if (num > 99)
	{
		error("limit of amount of heredocs reached");
		return 1;
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

int	adding_heredoc_into_file(int fd, bool expansion, char *delimiter)
{
	char	*heredoc_input;
	size_t	len;
	ssize_t	c_written;

	expansion = 0;
	while (42)
	{
		heredoc_input = readline("> ");
		if (!strncmp(delimiter, heredoc_input, HD_DELIMITER_LEN))
			break ;
//		var_expansion(heredoc_input, expansion);
		len = ft_strlen(heredoc_input);
		c_written = write(fd, heredoc_input, len);
		if (write_check(c_written, len))
			return (1);
		c_written = write(fd, "\n", 1);
		if (write_check(c_written, 1))
			return (1);
		free(heredoc_input);
	}
	return (0);
}

int	write_check(ssize_t c_written, size_t len)
{
	if (c_written == -1)
	{
		perror(NULL);
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
