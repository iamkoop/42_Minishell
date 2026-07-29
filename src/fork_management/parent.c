/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 15:38:08 by nildruon          #+#    #+#             */
/*   Updated: 2026/07/29 19:41:17 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	*create_id_array(int size)
{
	int *id_arr;

	id_arr = malloc(sizeof(int) * size);
	if(!id_arr)
		return(NULL);
	return(id_arr);
}

int parent(t_minishell *mini, t_single_linked_node	*envp)
{
	int i = 0;
	int *fork_id;

	mini->cmd_lst_size = ft_single_lstsize(mini->cmd_lst);
	fork_id = create_id_array(mini->cmd_lst_size);
	while(mini->cmd_lst)
    {
		if(i > 0)
			mini->prev_read_fd = mini->next_pipe_fds[0];
		if(mini->cmd_lst->next && pipe(mini->next_pipe_fds) == -1)
		{
			perror("pipe: ");
			exit(1);
		}
		fork_id[i] = fork();
		if(fork_id[i] == -1)
			perror("fork failed");
        if(fork_id[i] == 0)
		{
			mini->curr_cmd = (t_command	*)mini->cmd_lst;
			if(i == 0)
				first_child(mini, envp);
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
		waitpid(fork_id[i], &mini->exit_status, 0);
	if (WIFEXITED(mini->exit_status))
		return (WEXITSTATUS(mini->exit_status));
}
