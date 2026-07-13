/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_creation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 14:17:36 by nilsdruon         #+#    #+#             */
/*   Updated: 2026/07/13 21:00:38 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


// prev_pipe and nxt_pipe have the fds that are needed in that child and 
// therefore shouldnt be closed at the beginning of the child

static void first_child(int	*first_pipe, t_command	*cmd)
{
	close(first_pipe[0]);
	exec_command(NULL, NULL);
}
static void middle_children(int	prev_read_fd, int	*next_pipe_fds, t_command	*cmd)
{
	exec_command(NULL, NULL);
}

static void last_child(int	prev_read_fd, t_command	*cmd)
{
	close(prev_read_fd);
	exec_command(NULL, NULL);
}

static int parent(t_single_linked_node	*cmd_lst)
{
	int status = 0;
	int i = 0;
	int cmd_lst_size = ft_single_lstsize(cmd_lst);
	int id[cmd_lst_size];
	int next_pipe_fds[2];
	int prev_read_fd;
	t_command	*curr_cmd;

	while(cmd_lst)
    {
		if(i > 0)
			prev_read_fd = next_pipe_fds[0];
		if(cmd_lst->next && pipe(next_pipe_fds) == -1)
		{
			perror("pipe: ");
			exit(1);
		}
		id[i] = fork();
		if(id[i] == -1)
			perror("fork failed");
        if(id[i] == 0)
		{
			curr_cmd = (t_command	*)cmd_lst;
			if(i == 0)
			{
				first_child(next_pipe_fds, curr_cmd);
			}
			else if(!cmd_lst->next)
				last_child(prev_read_fd, curr_cmd);
			else
				middle_children(prev_read_fd, next_pipe_fds, curr_cmd);
		}
		else
		{
			if(i > 0)
				close(prev_read_fd);
		}
		i++;
		cmd_lst = cmd_lst->next;
 	}
	for(int i = 0; i < cmd_lst_size; i++)
		waitpid(id[i], &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
}

int main()
{
	
	return(1);
}
