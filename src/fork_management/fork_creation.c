/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_creation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 14:17:36 by nilsdruon         #+#    #+#             */
/*   Updated: 2026/07/14 19:09:10 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


// prev_pipe and nxt_pipe have the fds that are needed in that child and 
// therefore shouldnt be closed at the beginning of the child

static void first_child(t_minishell *mini)
{
	close(mini->next_pipe_fds[0]);
	mini->dupe2 = dup2(mini->next_pipe_fds[1], STDOUT_FILENO);
	exec_command(NULL, NULL);
}
static void middle_children(t_minishell *mini)
{
	mini->dupe1 = dup2(mini->prev_read_fd, STDIN_FILENO);
	close(mini->prev_read_fd);
	mini->dupe2 = dup2(mini->next_pipe_fds[1], STDOUT_FILENO);
	exec_command(NULL, NULL);
}

static void last_child(t_minishell *mini)
{
	mini->dupe1 = dup2(mini->prev_read_fd, STDIN_FILENO);
	exec_command(NULL, NULL);
}

static int parent(t_minishell *mini)
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
				first_child(mini);
			}
			else if(!mini->cmd_lst->next)
				last_child(mini);
			else
				middle_children(mini);
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
