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
#include "../../minishell.h"

int			here_doc(char *input, char **env, t_token_node *token_lst,
				t_token_iteri *iteri);
static int	prepare_delimiter(char *delimiter, t_token_node *token_lst,
				t_token_iteri *iteri, bool *expansion);
static int	heredoc_filename_creation(char *filename, char *input, t_token_iteri *iteri);
static int	adding_heredoc_into_file(int fd, bool expansion, char *delimiter);
static int	write_check(ssize_t c_written, size_t len);

int	here_doc(char *input, t_single_linked_node *env, t_token_node *token_lst,
		t_token_iteri *iteri)
{
	char	delimiter[HD_DELIMITER_LEN];
	char	filename[16];
	char	*buffer;
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
	buffer = adding_heredoc_into_file(fd, expansion, delimiter, env);
	if(buffer)
	{
		free(buffer);
		return (1);
	}
	close(fd);
	ft_strlcpy(token_lst[iteri->token - 1].token_str, filename, 17);
	return (0);
}

static int	prepare_delimiter(char *delimiter, t_token_node *token_lst,
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

static int	heredoc_filename_creation(char *filename, char *input, t_token_iteri *iteri)
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

static char	*adding_heredoc_into_file(int fd, bool expansion, char *delimiter, t_single_linked_node *env)
{
	char	*heredoc_input;
	ssize_t	c_written;
	char	word[WORD_AMOUNT][WORD_STR_SIZE];
	char	*tmp_heredoc_input;

	ft_bzero(word, WORD_AMOUNT * WORD_STR_SIZE);
	while (42)
	{
		heredoc_input = readline("> ");
		if (!ft_strncmp(delimiter, heredoc_input, HD_DELIMITER_LEN))
			break ;
		quote_rm_var_expan(heredoc_input, word, t_single_linked_node *env, true)
		if(expansion)
		{
			if(quote_rm_var_expan(heredoc_input, word, t_single_linked_node *env, true))
				return (heredoc_input);
			assert(word[1][0] == 0);
			tmp_heredoc_input = calloc(1, ft_strlen(word[0]) + 1);
			if(!tmp_heredoc_input)
				return (heredoc_input);
			ft_strlcpy(tmp_heredoc_input, word[0], ft_strlen(word[0]) + 1);
			free(heredoc_input);
			heredoc_input = tmp_heredoc_input;
		}
		c_written = write(fd, heredoc_input, ft_strlen(heredoc_input));
		if (write_check(c_written, len))
			return (heredoc_input);
		c_written = write(fd, "\n", 1);
		if (write_check(c_written, 1))
			return (heredoc_input);
		free(heredoc_input);
	}
	return (NULL);
}

static int	write_check(ssize_t c_written, size_t len)
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
