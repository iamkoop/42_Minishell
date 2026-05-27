/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:41:42 by nildruon          #+#    #+#             */
/*   Updated: 2026/05/27 16:32:03 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void env(t_single_linked_node	*envp)
{
	t_single_linked_node	*iter;
	t_env_var				*tmp;

	iter = envp;
	while (iter)
	{
		tmp = (t_env_var	*)iter->content;
		ft_putstr_fd(tmp->key,1);
		ft_putchar_fd('=',1);
		ft_putendl_fd(tmp->value,1);
		iter = iter->next;
	}
}