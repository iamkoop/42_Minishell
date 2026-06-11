/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:41:54 by nildruon          #+#    #+#             */
/*   Updated: 2026/06/12 00:13:34 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int unset(char	**argv, t_single_linked_node	*envp)
{
	t_single_linked_node	*prev;
	t_env_var	*node;
	int i;
	
	i = 1;
	prev = envp;
	while (envp)
	{
		node = (t_env_var *)envp->content;
		while (argv[i])
		{
			if(ft_strncmp()) 
			i++;
		}
		prev = envp;
		envp = envp->next;
	}
}
