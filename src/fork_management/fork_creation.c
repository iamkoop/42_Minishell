/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_creation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 14:17:36 by nilsdruon         #+#    #+#             */
/*   Updated: 2026/07/17 19:19:49 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


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
		else
			perror("dup2 fail in last child: ");
		return(0);
	}
	close(*fd);
	*fd = -42;
	return(1);
}

static void first_child(t_minishell *mini, t_single_linked_node	*envp)
{
	close(mini->next_pipe_fds[0]);
	if(!redirections(mini->curr_cmd->redir, mini))
		exit(1);
	if(mini->out < -1)
	{
		if(!final_redir(mini->next_pipe_fds[1], mini->redir_out, 0, 1))
			exit(1);
	}
	exec_command(mini->curr_cmd->argv, NULL);
}
static void middle_children(t_minishell *mini, t_single_linked_node	*envp)
{
	if(!redirections(mini->curr_cmd->redir, mini))
		exit(1);
	if(mini->in < -1)
	{
		if(!final_redir(mini->prev_read_fd, mini->redir_in, 1, 0))
			exit(1);
	}
	close(mini->prev_read_fd);
	if(mini->out < -1)
	{
		if(!final_redir(mini->next_pipe_fds[1], mini->redir_out, 1, 1))
			exit(1);
	}
	exec_command(mini->curr_cmd->argv, NULL);
}

static void last_child(t_minishell *mini, t_single_linked_node	*envp)
{
	if(!redirections(mini->curr_cmd->redir, mini))
		exit(1);
	if(mini->in < -1)
	{
		if(!final_redir(mini->prev_read_fd, mini->redir_in, 2, 0))
			exit(1);
	}
	exec_command(mini->curr_cmd->argv, NULL);
}

static int parent(t_minishell *mini, t_single_linked_node	*envp)
{
	int i = 0;
	int id[mini->cmd_lst_size];

	mini->cmd_lst_size = ft_single_lstsize(mini->cmd_lst);
	while(mini->cmd_lst)
    {
		if(i > 0)
			mini->prev_read_fd = mini->next_pipe_fds[0];
		if(mini->cmd_lst->next && pipe(mini->next_pipe_fds) == -1)
		{
			perror("pipe: ");
			exit(1);
		}
		id[i] = fork();
		if(id[i] == -1)
			perror("fork failed");
        if(id[i] == 0)
		{
			mini->curr_cmd = (t_command	*)mini->cmd_lst;
			if(i == 0)
			{
				first_child(mini, envp);
			}
			else if(!mini->cmd_lst->next)
				last_child(mini, envp);
			else
				middle_children(mini, envp);
		}
		else
		{
			if(i > 0)
				close(mini->prev_read_fd);
		}
		i++;
		mini->cmd_lst = mini->cmd_lst->next;
 	}
	for(int i = 0; i < mini->cmd_lst_size; i++)
		waitpid(id[i], &mini->status, 0);
	if (WIFEXITED(mini->status))
		return (WEXITSTATUS(mini->status));
}

int main()
{
	
	return(1);
}
