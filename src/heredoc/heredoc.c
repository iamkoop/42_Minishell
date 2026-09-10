/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 11:17:02 by bastalze          #+#    #+#             */
/*   Updated: 2026/09/10 10:10:34 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			here_doc(t_single_linked_node *env, t_minishell *mini,
				t_redir_list *redir_content);
static int	prepare_delimiter(char *delimiter, t_redir_list *redir_content,
				bool *expansion);
//static int	heredoc_filename_creation(char *filename, char *input,
//				t_token_iteri *iteri);
static int	creating_read_fd(t_redir_list *redir_content,
				t_minishell *mini);

int	here_doc(t_single_linked_node *env, t_minishell *mini,
		t_redir_list *redir_content)
{
	char	delimiter[HD_DELIMITER_LEN];
	bool	expansion;

	expansion = false;
	ft_bzero(delimiter, sizeof(char) * HD_DELIMITER_LEN);
	if (prepare_delimiter(delimiter, redir_content, &expansion))
		return (mini->exit_status = 1, 1);
	mini->heredoc_write_fd = open("/tmp", O_TMPFILE | O_WRONLY, 0600);
	if (mini->heredoc_write_fd == -1)
	{
		mini->exit_status = 1;
		return (perror("minshell: open heredoc failed"), 1);
	}
	if (adding_heredoc_into_file(mini, expansion, delimiter, env))
		return (close(mini->heredoc_write_fd), 1);
	if (creating_read_fd(redir_content, mini))
		return (close(mini->heredoc_write_fd), mini->exit_status = 1, 1);
	close(mini->heredoc_write_fd);
	return (0);
}

//There is a separate read fd because the file offset of the write fd would be
//where the last thing is written and then read would continue reading there.
//This way with the read fd you start reading from the beginning of the file.

static int	creating_read_fd(t_redir_list *redir_content,
				t_minishell *mini)
{
	int		read_fd;
	char	*path_to_write_fd;
	char	*full_path;
	char	*write_fd_char;

	path_to_write_fd = "/proc/self/fd/";
	write_fd_char = ft_itoa(mini->heredoc_write_fd);
	full_path = ft_strjoin(path_to_write_fd, write_fd_char);
	free(write_fd_char);
	if (!full_path)
		return (perror("minishell: malloc failure"), 1);
	read_fd = open(full_path, O_CLOEXEC, O_RDONLY);
	free (full_path);
	if (read_fd == -1)
		return (perror("minishell: open heredoc failed"), 1);
	redir_content->fd = read_fd;
	return (0);
}

// The /proc/self/fd is a symlink to the 
// /proc/[process id of current process]/fd which is a symlink that 
// points to an inode (data block) on the /tmp filesystem

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
