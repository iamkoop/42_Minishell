/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_envp_to_lst.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 17:09:29 by nildruon          #+#    #+#             */
/*   Updated: 2026/05/10 19:41:01 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	node->value = ft_calloc(key_len + 1, sizeof(char));
	if(!node->value)
		return(free(node->key), free(node), NULL);
	ft_strlcpy(node->value, str, str_len - key_len + 1);
	return (node);
}

t_env_var	*env_lst(char	**envp)
{
	int var;
	t_single_linked_node	*env_lst;
	t_single_linked_node	*tmp;
	
	if(!envp || !*envp)
		return (NULL);
	var = 0;
	tmp = create_env_node(envp[var]);
	if(!tmp)
		return(NULL);
	env_lst = ft_single_lstnew(tmp);
	if(!env_lst)
		return(NULL);
	tmp = env_lst;
	while (envp[++var])
	{
		tmp->next = malloc(sizeof(t_single_linked_node));
		if(!tmp->next)
			return (NULL);
		tmp->next->content=create_env_node(envp[var]);
		if(!tmp->next->content)
			return (NULL);
		tmp = tmp->next;
	}
}
