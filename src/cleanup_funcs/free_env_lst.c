/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_env_lst.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <bastalze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 17:50:42 by bastalze          #+#    #+#             */
/*   Updated: 2026/05/25 19:01:33 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    free_env_lst(t_single_linked_node *env)
{
	t_single_linked_node	*curr_node;

	curr_node = env;
	while(curr_node)
	{
		del_env_node_content(curr_node);
		curr_node = curr_node->next;
	}
}
