/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:41:42 by nildruon          #+#    #+#             */
/*   Updated: 2026/08/20 20:41:34 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int env(char	**input, t_single_linked_node	**envp)
{
	t_single_linked_node	*iter;
	t_env_var				*tmp;
	int len;

	len = 0;
	if(!envp)
		return (0);
	while(input[len])
		len++;
	if(len > 1)
		return(ft_putendl_fd("minishell: env: to many args usage: env", 2), 1);
	iter = *envp;
	while (iter)
	{
		tmp = (t_env_var	*)iter->content;
		ft_putstr_fd(tmp->key,1);
		ft_putchar_fd('=',1);
		ft_putendl_fd(tmp->value,1);
		iter = iter->next;
	}
	return(0);
}
