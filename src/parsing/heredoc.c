/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 11:17:02 by bastalze          #+#    #+#             */
/*   Updated: 2026/05/29 22:05:57 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../minishell.h"

int	here_doc(char **env, t_token_node *token_lst,
		t_token_iteri *iteri);
int	heredoc_filename_creation(char *filename);

int	here_doc(char **env, t_token_node *token_lst,
			t_token_iteri *iteri)
{
	int		len;
	char	delimiter[HD_DELIMITER_LEN];
	char	*heredoc_input;
	char	filename[16];
	bool	expansion;
	int		fd;
	ssize_t	c_written;

	env = NULL;	
	expansion = false;
	(iteri->i)++;
	ft_bzero(delimiter, sizeof(char) * HD_DELIMITER_LEN);
	len = ft_strlen(token_lst[iteri->token - 1].token_str);
	if (len >= HD_DELIMITER_LEN)
	{
		ft_putendl_fd("Exceeding memory limit: Heredoc delimiter \
				\nTo use a longer delimiter raise HD_DELIMITER_LEN \
				in minishell.h", 2);
		return (1);
	}
	ft_strlcpy(delimiter, token_lst[iteri->token - 1].token_str, len +1);
	if (strchr(delimiter, '\"') || strchr(delimiter, '\''))
	{
		expansion = true;
		quote_removal(delimiter);
	}
	if (heredoc_filename_creation(filename))
		return (1);
	fd = open((const char*)filename, O_WRONLY, O_CREAT, 0644);
	if (fd == -1)
	{
		perror(NULL);
		return (1);
	}
	while (42)
	{
		heredoc_input = readline("> ");
		if (!strncmp(delimiter, heredoc_input, HD_DELIMITER_LEN))
			break ;
//		var_expansion(heredoc_input, expansion);
		len = ft_strlen(heredoc_input);
		c_written = write(fd, heredoc_input, len);
		if (c_written == -1 || c_written != len)
		{
			perror(NULL);
			return (1);
		}
		free(heredoc_input);
	}
	close(fd);
	ft_strlcpy(token_lst[iteri->token - 1].token_str, filename, 17);
	return (0);
}

int	heredoc_filename_creation(char *filename)
{
	static int	num;
	char		*start;

	start = ".hd/File_";
	memcpy(filename, start, 9);
	if (num > 99)
	{
		write(2, "You exceeded the limit of 100 heredocs\n", 39);
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
	return (0);
}
