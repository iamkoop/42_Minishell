/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_env_node_content.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 17:50:42 by nildruon          #+#    #+#             */
/*   Updated: 2026/08/22 18:54:48 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void del_env_node_content(void	*content)
{
	t_env_var *node;

	node = (t_env_var	*)content;
	if(!content)
		return	;
	free(node->key);
	free(node->value);
	free(node);
}
