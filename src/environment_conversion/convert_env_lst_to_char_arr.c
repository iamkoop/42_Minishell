/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_env_lst_to_char_arr.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 19:54:45 by nildruon          #+#    #+#             */
/*   Updated: 2026/05/26 15:35:36 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		env[i] = ft_strjoin_three(tmp->key, "=", tmp->value);
		if(!env[i])
			return(ft_free_unfished_array_of_strings(env, i));
		i++;
		lst = lst->next;
	}
	env[i] = NULL;
	return(env);
}
