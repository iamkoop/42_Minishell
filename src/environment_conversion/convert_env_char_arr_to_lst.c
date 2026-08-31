/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_env_char_arr_to_lst.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 17:09:29 by nildruon          #+#    #+#             */
/*   Updated: 2026/08/31 16:05:18 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_env_var	*create_node_and_key(t_create_env_node_vars	*vars, char *s)
{
	t_env_var	*node;

	node = malloc(sizeof(t_env_var));
	if(!node)
		return(NULL);
	node->key = ft_calloc(vars->key_len + 1, sizeof(char));
	if(!node->key)
		return(free(node), NULL);
	ft_strlcpy(node->key, s, vars->key_len + 1);
	return(node);
}

t_env_var	*create_env_node(char	*str)
{
	t_env_var	*node;
	t_create_env_node_vars	vars;

	vars.str_len = ft_strlen(str);
	vars.key_len = 0;
	vars.no_equals = 0;
	while (str[vars.key_len] && str[vars.key_len] != '=')
		vars.key_len++;
	if (str[vars.key_len] != '=')
		vars.no_equals = 1;
	node = create_node_and_key(&vars, str);
	if(!node)
		return(NULL);
	str += (vars.key_len + 1);
	if(!vars.no_equals)
	{
		node->value = ft_calloc(vars.str_len - vars.key_len + 1, sizeof(char));
		if(!node->value)
			return(free(node->key), free(node), NULL);
		ft_strlcpy(node->value, str, vars.str_len - vars.key_len + 1);
	}
	else
		node->value = NULL;
	return (node);
}

t_single_linked_node	*env_to_lst(char	**envp)
{
	t_single_linked_node	*env_lst;
	t_single_linked_node	*node;
	t_env_var	*curr_content;
	int var;
	
	if(!envp || !*envp)
		return (NULL);
	var = 0;
	env_lst = NULL;
	while (envp[var])
	{
		curr_content = create_env_node(envp[var]);
		if(!curr_content)
			return(ft_single_lstclear(&env_lst, del_env_node_content), NULL);
		node = ft_single_lstnew(curr_content);
		if(!node)
		{
			del_env_node_content(curr_content);
			return(ft_single_lstclear(&env_lst, del_env_node_content), NULL);
		}
		ft_lstadd_back_single_linked(&env_lst, node);
		var++;
	}
	return (env_lst);
}
