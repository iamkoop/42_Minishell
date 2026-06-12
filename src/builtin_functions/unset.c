/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:41:54 by nildruon          #+#    #+#             */
/*   Updated: 2026/06/12 18:15:53 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void remove_node(t_single_linked_node	**curr,t_single_linked_node	**prev)
{
	t_single_linked_node	*tmp;

	tmp = (*curr)->next;
	if(prev)
		(*prev)->next = (*curr)->next;
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
	
	i = 0;
	prev = NULL;
	while (input[i])
	{
		curr = *envp;
		bigger = ft_strlen(input[i]);
		while (curr)
		{
			node = (t_env_var	*)curr->content;
			if(ft_strlen(node->key) > bigger)
				bigger = ft_strlen(node->key);
			if(!ft_strncmp(node->key, input[i], bigger))
				remove_node(&curr, &prev);
			prev = curr;
			curr = curr->next;
		}
		i++;
	}
	return(0);
}
