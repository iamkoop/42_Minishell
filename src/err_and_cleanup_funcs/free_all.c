/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 16:46:40 by bastalze          #+#    #+#             */
/*   Updated: 2026/09/05 13:40:26 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	free_all(t_single_linked_node	*env, t_minishell	*mini)
{
	free_command_struct(mini->cmd_lst);
	free_env_lst(env);
}
