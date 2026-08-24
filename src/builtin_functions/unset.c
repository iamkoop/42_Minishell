/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:41:54 by username          #+#    #+#             */
/*   Updated: 2026/08/25 01:41:04 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void remove_node(t_single_linked_node * *curr,
	t_single_linked_node	**prev,
t_single_linked_node	**envp)
{
	t_single_linked_node	*tmp;

	tmp = (*curr)->next;
	if (!*prev)
		*envp = tmp;
	else
		(*prev)->next = tmp;
	del_env_node_content((*curr)->content);
	free(*curr);
	*curr = tmp;
}

int	unset(char **input, t_single_linked_node **envp)
{
	t_single_linked_node	*prev;
	t_single_linked_node	*curr;
	t_env_var				*node;
	int						i;

	i = 1;
	while (input[i])
	{
		curr = *envp;
		prev = NULL;
		while (curr)
		{
			node = (t_env_var *) curr->content;
			if (!ft_strcmp(node->key, input[i]))
			{
				remove_node(&curr, &prev, envp);
				break ;
			}
			prev = curr;
			curr = curr->next;
		}
		i++;
	}
	return (0);
}
