/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_processes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 16:39:36 by nildruon          #+#    #+#             */
/*   Updated: 2026/08/09 19:47:23 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// prev_pipe and nxt_pipe have the fds that are needed in that child and 
// therefore shouldnt be closed at the beginning of the child

static int final_redir(int	*fd, int	*final_fd, int child, int file_eno)
{
	*final_fd = dup2(*fd, file_eno);
	if(*final_fd == -1)
	{
		if(child == 0)
			perror("dup2 fail in first child: ");
		else if(child == 1)
			perror("dup2 fail in middle children: ");
		else if(child == 2)
			perror("dup2 fail in last child: ");
		else
			perror("dup2 fail in non-builtin: ");
		return(0);
	}
	if (*fd != file_eno)
		close_fd(fd);
	*fd = -42;
	return(1);
}
/*
	0 : first child (NEED to ridirect to out)
	1 : middle children (NEED to ridirect in && out)
	2 : last child (NEED to ridirect in)
	3 : normal cmd that was forked (no final redirections)
*/

void child_process(t_minishell *mini, t_single_linked_node	*envp, int close_read, int child_type)
{
	if(close_read)
		close_fd(&mini->next_pipe_fds[0]);
	if(!redirections(mini->curr_cmd->redir, mini))
		exit(1);
	if(mini->in < -1 && (child_type != 0 && child_type != 3))
	{
		if(!final_redir(&mini->prev_read_fd, &mini->redir_in, child_type, STDIN_FILENO))
			exit(1);
	}
	if(mini->out < -1 && (child_type != 2 && child_type != 3))
	{
		if(!final_redir(&mini->next_pipe_fds[1], &mini->redir_out, child_type, STDOUT_FILENO))
			exit(1);
	}
	close_fd(&mini->next_pipe_fds[1]);
	exec_command(mini->curr_cmd->argv, envp);
	exit(mini->exit_status);
}
