/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_from_lst.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 01:08:18 by nildruon          #+#    #+#             */
/*   Updated: 2026/08/20 18:57:30 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_single_linked_node	*get_env_from_lst(char	*to_find, t_single_linked_node	*envp)
{
	t_env_var	*tmp;

	if (!envp)
        return (NULL);
	while (envp)
	{
		tmp = (t_env_var	*)envp->content;
		if (!ft_strcmp(to_find, tmp->key))
			return(envp);
		envp = envp->next;
	}
	return (NULL);
}
