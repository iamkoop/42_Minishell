/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_from_lst.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 01:08:18 by nildruon          #+#    #+#             */
/*   Updated: 2026/06/11 21:14:08 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_single_linked_node	*get_env_from_lst(char	*to_find, t_single_linked_node	*envp)
{
	t_env_var	*tmp;
	size_t		key_len;
	size_t		to_find_len;

	if (!envp)
        return (NULL);
	to_find_len = ft_strlen(to_find);
	while (envp != NULL)
	{
		tmp = (t_env_var	*)envp->content;
		key_len = ft_strlen(tmp->key);
		if (to_find_len == key_len && !ft_strncmp(to_find, tmp->key, key_len))
			return(envp);
		envp = envp->next;
	}
	return (NULL);
}
