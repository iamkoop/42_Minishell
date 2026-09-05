/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:41:48 by username          #+#    #+#             */
/*   Updated: 2026/09/05 12:59:47 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_valid_identifier(char *input)
{
	int	i;

	if (!ft_strlen(input))
		return (0);
	i = 0;
	if (!(ft_isalpha(input[i]) || input[i] == '_'))
		return (0);
	i++;
	while (input[i] && input[i] != '=')
	{
		if (!(ft_isalnum(input[i]) || input[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

t_env_var	*fill_var_content(char *input)
{
	t_env_var	*env_var;
	int			len;

	env_var = malloc(sizeof(t_env_var));
	if (!env_var)
		return (NULL);
	len = 0;
	while (input[len] && input[len] != '=')
		len++;
	env_var->key = malloc(sizeof(char) * len + 1);
	if (!env_var->key)
		return (free(env_var), NULL);
	ft_strlcpy(env_var->key, input, len + 1);
	if (input[len] == '=')
	{
		input += len + 1;
		env_var->value = ft_strdup(input);
		if (!env_var->value)
			return (free(env_var->key), free(env_var), NULL);
	}
	else
		env_var->value = NULL;
	return (env_var);
}

static t_single_linked_node	*create_var(char *input)
{
	t_single_linked_node	*node;

	if (!is_valid_identifier(input))
	{
		ft_putstr_fd("minishell: export: ", 2);
		ft_putstr_fd(input, 2);
		ft_putendl_fd(": not a valid identifier", 2);
		return (NULL);
	}
	node = malloc(sizeof(t_single_linked_node));
	if (!node)
		return (NULL);
	node->content = fill_var_content(input);
	if (!node->content)
	{
		err_msg("export", NULL, "malloc in fill_var_content failed");
		return (free(node), NULL);
	}
	node->next = NULL;
	return (node);
}

static int	print_sorted(t_single_linked_node *envp)
{
	if (envp)
		printf("sorted");
	return (0);
}

static int	search_for_node(t_single_linked_node *node,
	t_single_linked_node *envp)
{
	t_env_var	*curr_content;
	t_env_var	*node_content;

	node_content = (t_env_var *) node->content;
	while (envp)
	{
		curr_content = (t_env_var *) envp->content;
		if (ft_strcmp(node_content->key, curr_content->key) == 0)
		{
			if (node_content->value)
			{
				free(curr_content->value);
				curr_content->value = ft_strdup(node_content->value);
			}
			if (node_content->value && !curr_content->value)
			{
				ft_putendl_fd("minishell: export: malloc fail node search", 2);
				return (del_env_node_content(node->content), free(node), 0);
			}
			return (del_env_node_content(node->content), free(node), 1);
		}
		envp = envp->next;
	}
	return (2);
}

int	export(char **input, t_single_linked_node **envp)
{
	t_single_linked_node	*node;
	int						ret;
	int						i;

	i = 1;
	ret = 0;
	if (!input[1])
		return (print_sorted(*envp));
	while (input[i])
	{
		node = create_var(input[i]);
		if (!node)
			return (1);
		ret = search_for_node(node, *envp);
		if (!ret)
			return (1);
		if (ret == 2)
			ft_lstadd_back_single_linked(envp, node);
		i++;
	}
	return (0);
}
