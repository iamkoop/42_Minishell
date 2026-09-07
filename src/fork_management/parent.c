/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 15:38:08 by nildruon          #+#    #+#             */
/*   Updated: 2026/09/07 18:11:25 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	*create_id_array(int size)
{
	int	*id_arr;

	id_arr = malloc(sizeof(int) * size);
	if (!id_arr)
		return (NULL);
	return (id_arr);
}

static void	wait_for_children(t_minishell *mini, int *fork_id, int size)
{
	int		i;
	int		status;
	bool	printed_nl;

	printed_nl = false;
	i = 0;
	status = 0;
	while (i < size)
	{
		waitpid(fork_id[i], &status, 0);
		if (WIFEXITED(status))
			mini->exit_status = WEXITSTATUS(status);
	//	printf("exit status: %d", mini->exit_status);
	if (printed_nl == false)
	{
		if (mini->exit_status == 130)
		{
			ft_putendl_fd("", 1);
			printed_nl = true;
		}
		if (mini->exit_status == 131)
		{
			ft_putendl_fd("", 1);
			printed_nl = true;
		}
	}
		i++;
	}
}

static int	fork_options(int *fork_id, int size, t_minishell	*mini,
				t_single_linked_node	**envp)
{

	if (fork_id[size] == -1)
		return (mini->exit_status = 1, perror("fork in parent failed"), 0);
	if (fork_id[size] == 0)
	{
		set_sigquit_to_default();
		set_sigint_to_default();
		free(fork_id);
		mini->curr_cmd = (t_command *)mini->cmd_lst->content;
		if (mini->cmd_lst_size == 1)
			child_process(mini, envp, 0, 3);
		if (size == 0)
			child_process(mini, envp, 1, 0);
		else if (!mini->cmd_lst->next)
			child_process(mini, envp, 0, 2);
		else
			child_process(mini, envp, 1, 1);
		exit(0);
	}
	if (mini->cmd_lst->next)
		close_fd(&mini->next_pipe_fds[1]);
	if (size > 0)
		close_fd(&mini->prev_read_fd);
	return (1);
}

void	parent(t_minishell *mini, t_single_linked_node	**envp)
{
	int	size;
	int	*fork_id;

	size = 0;
	mini->cmd_lst_size = ft_single_lstsize(mini->cmd_lst);
	fork_id = create_id_array(mini->cmd_lst_size);
	while (mini->cmd_lst)
	{
		if (size > 0)
			mini->prev_read_fd = mini->next_pipe_fds[0];
		if (mini->cmd_lst->next && pipe(mini->next_pipe_fds) == -1)
		{
			perror("pipe: ");
			exit(1);
		}
		ignore_sigint();
		fork_id[size] = fork();
		if (!fork_options(fork_id, size, mini, envp))
			break ;
		mini->cmd_lst = mini->cmd_lst->next;
		size++;
	}
	wait_for_children(mini, fork_id, size);
	signal_strl_c();
	free(fork_id);
}
