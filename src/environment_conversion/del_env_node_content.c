/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_env_node_content.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 17:50:42 by nildruon          #+#    #+#             */
/*   Updated: 2026/09/10 10:10:27 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	del_env_node_content(void	*content)
{
	t_env_var	*node;

	node = (t_env_var *)content;
	if (!content)
		return ;
	if (node->key)
	{
		free(node->key);
		node->key = NULL;
	}
	if (node->value)
	{
		free(node->value);
		node->value = NULL;
	}
	free(node);
	node = NULL;
}
