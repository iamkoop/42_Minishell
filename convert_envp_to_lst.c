/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_envp_to_lst.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 17:09:29 by nildruon          #+#    #+#             */
/*   Updated: 2026/05/08 16:45:52 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env_var	*create_env_node(char	*str)
{
	t_env_var	*node;
	int key_len;
	int i;

	key_len = 0;
	i = 0;
	while (str[key_len] && str[key_len++] != '=')
		key_len++;
	node = malloc(sizeof(t_env_var));
	if(!node)
		return(NULL);
	node->key = malloc((key_len + 1) * sizeof(char));
	ft_strlcpy(node->key,str,key_len);
	
}

t_env_var	*env_lst(char	**envp)
{
	int var;
	t_env_var	*env_var;
	
	if(!envp || !*envp)
		return (NULL);
	var = 0;
	while (envp[var])
	{
		env_var = create_env_node(envp[var]);
		var++;
	}
	
}
