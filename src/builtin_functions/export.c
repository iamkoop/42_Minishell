/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:41:48 by nildruon          #+#    #+#             */
/*   Updated: 2026/08/20 16:42:36 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//TODO: Parsing from what comes (not a valid identifier) IMPORTANT!!!!!
//TODO: (BONUS) if you export a an existing var then suould replace the value with the new value
//TODO: (BONUS) could appending (+=)
//TODO: add the sorted print and fix 2 cases from tester

static int	is_valid_identifier(char	*input)
{
	int first_equals;
	int i;

	if(!ft_strlen(input))
		return(0);
	i = 0;
	if(!(ft_isalpha(input[i]) || input[i] == '_'))
		return(0);
	i++;
	first_equals = 1;
	while (input[i] && input[i] != '=')
	{
		if(!(ft_isalnum(input[i]) || input[i] == '_'))
			return(0);
		i++;
	}
	return(1);
}

t_env_var	*fill_var_content(char	*input)
{
	t_env_var	*env_var;
    int len;

	env_var = malloc(sizeof(t_env_var));
	if(!env_var)
        return(NULL);
    len = 0;
    while(input[len] && input[len] != '=')
		len++;
	env_var->key = malloc(sizeof(char) * len + 1);
    if(!env_var->key)
        return(NULL);
    ft_strlcpy(env_var->key, input, len + 1);
	if(input[len] == '=')
	{
		
		input += len + 1;
		env_var->value = ft_strdup(input);
		if(!env_var->value)
			return(NULL);
	}
	else
		env_var->value = NULL;
	return(env_var);
}

static t_single_linked_node	*create_var(char    *input)
{
    t_single_linked_node    *node;

    if(!is_valid_identifier(input))
    {
        ft_putstr_fd("minishell: export: ", 2);
		ft_putstr_fd(input, 2);
        ft_putendl_fd(": not a valid identifier", 2);
		return(NULL);  
    }
	node = malloc(sizeof(t_single_linked_node));
	if(!node)
		return(NULL);
	node->content = fill_var_content(input);
    if(!node->content)
    {
		ft_putendl_fd("minishell: export: malloc in fill_var_content failed", 2);
		return(NULL);
	}
    node->next = NULL;
    return(node);
}


static int print_sorted(t_single_linked_node *envp)
{
	//search for next string to print
	if(envp)
		printf("sorted");
	return(0);
}

static int search_for_node(t_single_linked_node *node, t_single_linked_node	*envp)
{
	t_env_var	*curr_content;
	t_env_var	*node_content;

	node_content = (t_env_var	*)node->content;
	while (envp)
	{
		curr_content = (t_env_var	*)envp->content;
		if(ft_strcmp(node_content->key, curr_content->key) == 0)
		{
			free(curr_content->value);
			curr_content->value = ft_strdup(node_content->value);
			if(!curr_content->value)
			{
				ft_putendl_fd("minishell: export: malloc fail node search", 2);
				return(0);
			}
			del_env_node_content(node->content);
			free(node);
			return(1);
		}
		envp = envp->next;
	}
	return(2);
}

int export(char **input, t_single_linked_node **envp)
{
	t_single_linked_node    *node;
	int ret;
	int	i;

	i = 1;
	ret = 0;
	if(!input[1])
		return(print_sorted(*envp));
	while (input[i])
	{
		node = create_var(input[i]);
        if(!node)
            return(1);
		ret = search_for_node(node, *envp);
		if(!ret)
			return(1);
		if(ret == 2)
			ft_lstadd_back_single_linked(envp, node);
		i++;
	}
	return(0);
}
