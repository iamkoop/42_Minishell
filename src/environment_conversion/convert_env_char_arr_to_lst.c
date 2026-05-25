/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_env_char_arr_to_lst.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 17:09:29 by nildruon          #+#    #+#             */
/*   Updated: 2026/05/25 21:38:52 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_env_var	*create_env_node(char	*str)
{
	t_env_var	*node;
	int			key_len;
	int			str_len;

	str_len = ft_strlen(str);
	key_len = 0;
	while (str[key_len] && str[key_len] != '=')
		key_len++;
	node = malloc(sizeof(t_env_var));
	if(!node)
		return(NULL);
	node->key = ft_calloc(key_len + 1, sizeof(char));
	if(!node->key)
		return(free(node), NULL);
	ft_strlcpy(node->key, str, key_len + 1);
	str += (key_len + 1);
	node->value = ft_calloc(str_len - key_len + 1, sizeof(char));
	if(!node->value)
		return(free(node->key), free(node), NULL);
	ft_strlcpy(node->value, str, str_len - key_len + 1);
	return (node);
}

static int help_convert_env_to_lst(t_single_linked_node **current, char	*envp)
{
	(*current)->next = ft_single_lstnew(NULL);
	if(!(*current)->next)
		return(0);
	(*current) = (*current)->next;
	(*current)->content = create_env_node(envp);
	if(!(*current)->content)
		return(0);
	return(1);
}

t_single_linked_node	*env_to_lst(char	**envp)
{
	int var;
	t_single_linked_node	*env_lst;
	t_single_linked_node	*current;
	
	if(!envp || !*envp)
		return (NULL);
	var = 0;
	current = ft_single_lstnew(NULL);
	if(!current)
		return (NULL);
	current->content = create_env_node(envp[var++]);
	if(!current->content)
		return (free(current), NULL);
	env_lst = current;	
	while (envp[var])
	{
		if(!help_convert_env_to_lst(&current, envp[var]))
			return(ft_single_lstclear(&env_lst, del_env_node_content), NULL);
		var++;
	}
	current->next = NULL;
	return (env_lst);
}
