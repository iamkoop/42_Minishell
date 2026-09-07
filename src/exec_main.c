/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 14:56:59 by nildruon          #+#    #+#             */
/*   Updated: 2026/09/07 12:29:05 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_main(t_minishell	*mini, t_single_linked_node	*cmd_lst,
			t_single_linked_node	**envp)
{
	mini->cmd_lst = cmd_lst;
	if (!mini->cmd_lst)
		return ;
	mini->cmd_lst_size = ft_single_lstsize(mini->cmd_lst);
	if (!mini->cmd_lst_size)
		return ;
	mini->curr_cmd = (t_command *)cmd_lst->content;
	if (mini->cmd_lst_size == 1 && mini->curr_cmd->argv
		&& is_builtin(mini->curr_cmd->argv[0]))
		builtin_redir_special_case(mini, envp);
	else
		parent(mini, envp);
}
