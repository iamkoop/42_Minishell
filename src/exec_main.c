/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 14:56:59 by nildruon          #+#    #+#             */
/*   Updated: 2026/08/03 13:49:52 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void exec_main(t_minishell *mini, t_single_linked_node	*cmd_lst, t_single_linked_node	*envp)
{
	mini->cmd_lst = cmd_lst;
	if(!mini->cmd_lst)
		return ;
	mini->cmd_lst_size = ft_single_lstsize(cmd_lst);
	if(!mini->cmd_lst_size)
		return ;
	else if(mini->cmd_lst_size == 1)
	{
		mini->curr_cmd = (t_command	*)cmd_lst->content;
		if(is_builtin(mini->curr_cmd->argv[0]))
		{
			
		}
		else
			child_process(mini, envp, 0, 3);
	}
	else
		parent(mini, envp);
}
