/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_env_lst.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 17:50:42 by bastalze          #+#    #+#             */
/*   Updated: 2026/09/07 14:17:28 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	free_env_lst(t_single_linked_node	*env)
{
	t_single_linked_node	*curr_node;
	t_single_linked_node	*next_node;

	curr_node = env;
	while (curr_node)
	{
		next_node = curr_node->next;
		del_env_node_content(curr_node->content);
		free(curr_node);
		curr_node = next_node;
	}
	env = NULL;
}
