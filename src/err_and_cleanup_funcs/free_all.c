/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 16:46:40 by bastalze          #+#    #+#             */
/*   Updated: 2026/08/21 18:53:40 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void    free_all(t_single_linked_node *env, t_minishell *mini)
{
    free_command_struct(mini->cmd_lst);
    free_env_lst(env);
}