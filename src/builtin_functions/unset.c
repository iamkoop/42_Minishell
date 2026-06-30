/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:41:54 by nildruon          #+#    #+#             */
/*   Updated: 2026/06/30 00:45:16 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void remove_node(t_single_linked_node	**curr,
						t_single_linked_node	**prev,
						t_single_linked_node	**envp)
{
	t_single_linked_node	*tmp;

	tmp = (*curr)->next;
	if(!*prev)
		*envp = tmp;
	else
		(*prev)->next = tmp;
	del_env_node_content((*curr)->content);
	free(*curr);
	*curr = tmp;
}

int unset(char	**input, t_single_linked_node	**envp)
{
	t_single_linked_node	*prev;
	t_single_linked_node	*curr;
	t_env_var	*node;
	size_t		bigger;
	int i;
	
	i = 1;
	while (input[i])
	{
		curr = *envp;
		prev = NULL;
		bigger = ft_strlen(input[i]);
		while (curr)
		{
			node = (t_env_var	*)curr->content;
			if(ft_strlen(node->key) > bigger)
				bigger = ft_strlen(node->key);
			if(!ft_strncmp(node->key, input[i], bigger))
			{
				remove_node(&curr, &prev, envp);
				break;
			}
			prev = curr;
			curr = curr->next;
		}
		i++;
	}
	return(0);
}
