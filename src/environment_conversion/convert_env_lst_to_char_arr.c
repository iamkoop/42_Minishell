/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_env_lst_to_char_arr.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 19:54:45 by nildruon          #+#    #+#             */
/*   Updated: 2026/08/22 18:54:56 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**env_to_char_arr(t_single_linked_node	*lst)
{
	char		**env;
	t_env_var	*tmp;
	int			i;
	
	if(!lst)
		return(NULL);
	env = malloc(sizeof(char	*) * (ft_single_lstsize(lst) + 1));
	if(!env)
		return(NULL);
	i = 0;
	while (lst)
	{
		tmp = (t_env_var	*)lst->content;
		if(!tmp->value || tmp->value[0] == '\0')
			env[i] = ft_strdup(tmp->key);
		else
			env[i] = ft_strjoin_three(tmp->key, "=", tmp->value);
		if(!env[i])
			return(ft_free_unfished_array_of_strings(env, i));
		i++;
		lst = lst->next;
	}
	env[i] = NULL;
	return(env);
}
