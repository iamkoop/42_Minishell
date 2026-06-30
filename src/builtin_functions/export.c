/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:41:48 by nildruon          #+#    #+#             */
/*   Updated: 2026/06/30 04:08:21 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//TODO: Parsing from what comes (not a valid identifier) IMPORTANT!!!!!
//TODO: (BONUS) if you export a an existing var then suould replace the value with the new value
//TODO: (BONUS) could appending (+=)

static void compare_content(t_single_linked_node **curr_smallest,
                            t_single_linked_node *prev_smallest,
                            t_single_linked_node *curr_node)
{
    t_env_var *smllst;
    t_env_var *prev_smllst;
    t_env_var *curr;

    smllst = (t_env_var *)(*curr_smallest)->content;
	if(prev_smallest)
    	prev_smllst = (t_env_var *)prev_smallest->content;
    curr = (t_env_var *)curr_node->content;
    if(!prev_smallest)
    {
        if(ft_strncmp(curr->key, smllst->key,
            ft_get_biggest_s(curr->key, smllst->key)) < 0)
            *curr_smallest = curr_node;
        return;
    }
    if((ft_strncmp(curr->key, smllst->key,
        ft_get_biggest_s(curr->key, smllst->key)) < 0)
        && (ft_strncmp(curr->key, prev_smllst->key,
            ft_get_biggest_s(curr->key, prev_smllst->key)) > 0))
        *curr_smallest = curr_node;
}

static int print_sorted_env(t_single_linked_node *envp)
{
    t_print_sorted_env_vars vars;

    vars = (t_print_sorted_env_vars){0};
    vars.lst_len = ft_single_lstsize(envp);
    while (vars.curr_cnt < vars.lst_len)
    {
        vars.tmp_lst = envp;
        while(vars.tmp_lst)
        {
            if(!vars.prev_smllst_alpha && !vars.curr_smllst_alpha)
                vars.curr_smllst_alpha = vars.tmp_lst;
            compare_content(&vars.curr_smllst_alpha, vars.prev_smllst_alpha, vars.tmp_lst);
            vars.tmp_lst = vars.tmp_lst->next;
        }
        vars.tmp_env_var = (t_env_var   *)vars.curr_smllst_alpha;
        printf("declare -x %s=\"%s\"", vars.tmp_env_var->key, vars.tmp_env_var->value);
        vars.prev_smllst_alpha = vars.curr_smllst_alpha;
        vars.curr_cnt++;
    }
	return (0);
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
		input += 1;
    input += len;
    len = 0;
    while (input[len])
		len++;
	if(!ft_strlen(input))
	{
		env_var->value = NULL;
		return(env_var);
	}
	env_var->value = malloc(sizeof(char) * len + 1);
    if(!env_var->value)
        return(NULL);
    ft_strlcpy(env_var->value, input, len + 1);
    return(env_var);
}

t_single_linked_node    *create_var(char    *input)
{
    t_single_linked_node    *node;

    if(!ft_strlen(input))
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

static int search_for_node(t_single_linked_node *node, t_single_linked_node *envp)
{
	t_env_var	*node_content;
	t_env_var	*curr_content;
	size_t bigger;

	node_content = (t_env_var	*)node->content;
	while(envp)
	{
		curr_content = (t_env_var	*)envp->content;
		bigger = ft_get_biggest_s(curr_content->key, node_content->key);
		if(ft_strncmp(curr_content->key, node_content->key, bigger) == 0)
		{
			free(curr_content->value);
			curr_content->value = ft_strdup(node_content->value);
			del_env_node_content(node->content);
			free(node);
			if(!node_content->value)
			{
				ft_putendl_fd("minishell: export: malloc fail node search", 2);
				return(0);
			}
			return(1);
		}
		envp = envp->next;
	}
	return(2);
}

int export(char **input, t_single_linked_node *envp)
{
    t_single_linked_node    *node;
    int i;
    int node_search;

    if(!envp)
        return(1);
    if(input[0] && !input[1])
        return(print_sorted_env(envp));
    i = 1;
    while (input[i])
    {
        node = create_var(input[i]);
        if(!node)
            return(ft_putendl_fd("minishell: export: node malloc fail", 2), 1);
		node_search = search_for_node(node, envp);
        if(!node_search)
            return(1);
        if(node_search == 2)
            ft_lstadd_back_single_linked(&envp, node);
        i++;
    }
    return(0);
}
