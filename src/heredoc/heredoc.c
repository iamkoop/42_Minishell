/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 11:17:02 by bastalze          #+#    #+#             */
/*   Updated: 2026/08/27 16:56:51 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int			here_doc(t_single_linked_node *env, t_minishell *mini,
				t_redir_list *redir_content);
static int	prepare_delimiter(char *delimiter, t_redir_list *redir_content,
				bool *expansion);
//static int	heredoc_filename_creation(char *filename, char *input,
//				t_token_iteri *iteri);
static int	creating_read_fd(char *filename, t_redir_list *redir_content,
				t_minishell *mini);

int	here_doc(t_single_linked_node *env, t_minishell *mini,
		t_redir_list *redir_content)
{
	char	delimiter[HD_DELIMITER_LEN];
	char	*filename;
	bool	expansion;

	expansion = false;
	ft_bzero(delimiter, sizeof(char) * HD_DELIMITER_LEN);
	if (prepare_delimiter(delimiter, redir_content, &expansion))
		return (1);
	filename = "/tmp/tmp.JuvA2Vpt5q";
//	if (heredoc_filename_creation(filename, input, iteri))
//		return (1);
//	mini->heredoc_write_fd = open(".", O_TMPFILE | O_WRONLY |
//		O_CLOEXEC, 0600);
	mini->heredoc_write_fd = open((const char *)filename, O_WRONLY
			| O_CLOEXEC | O_EXCL | O_CREAT, 0600);
	if (mini->heredoc_write_fd == -1)
		return (perror("minshell: open heredoc failed"), 1);
	if (adding_heredoc_into_file(mini, expansion, delimiter, env))
		return (unlink(filename), close(mini->heredoc_write_fd), 1);
	if (creating_read_fd(filename, redir_content, mini))
		return (unlink(filename), close(mini->heredoc_write_fd), 1);
	close(mini->heredoc_write_fd);
	unlink(filename);
	return (0);
}

//There is a separate read fd because the file offset of the write fd would be
//where the last thing is written and then read would continue reading there.
//This way with the read fd you start reading from the beginning of the file.

static int	creating_read_fd(char *filename, t_redir_list *redir_content,
				t_minishell *mini)
{
	int		read_fd;

	(void)mini;
	read_fd = open((const char *) filename, O_RDONLY);
	if (read_fd == -1)
		return (unlink(filename), perror("minishell: open heredoc failed"), 1);
	redir_content->fd = read_fd;
	return (0);
}
/*
static int	file_path_read_fd()
{

}
*/

static int	prepare_delimiter(char *delimiter, t_redir_list *redir_content,
				bool *expansion)
{
	size_t	len;

	len = ft_strlen(redir_content->filename);
	if (len + 2 >= HD_DELIMITER_LEN)
	{
		error("exceeding memory limit: Heredoc delimiter \
				\nRaise HD_DELIMITER_LEN in minishell.h");
		return (1);
	}
	ft_strlcpy(delimiter, redir_content->filename, len +1);
	if (ft_strchr(delimiter, '\"') || ft_strchr(delimiter, '\''))
		quote_removal(delimiter);
	else
		*expansion = true;
	len = ft_strlen(delimiter);
	delimiter[len] = '\n';
	delimiter[len + 1] = 0;
	return (0);
}
/*
static int	heredoc_filename_creation(char *filename, char *input,
				t_token_iteri *iteri)
{
	static int	num;
	char		*start;

	start = ".hd/File_";
	ft_memcpy(filename, start, 9);
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
*/