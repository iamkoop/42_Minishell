/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inializing_minishell.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 14:57:36 by bastalze          #+#    #+#             */
/*   Updated: 2026/09/09 19:31:17 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int						initializing_minishell(char **envp);
static int				update_shell_level(t_single_linked_node *env);

int	initializing_minishell(char **envp)
{
	t_single_linked_node	**env;
	t_single_linked_node	*env_head;
	t_minishell				mini;

	ft_bzero(&mini, sizeof(t_minishell));
	if (!envp || !envp[0])
	{
		env_head = default_env();
		if (!env_head)
			return (1);
	}
	else
	{
		env_head = env_to_lst(envp);
		if (!env_head)
			return (1);
		if (update_shell_level(env_head))
			return (free_env_lst(env_head), 1);
	}
	env = &env_head;
	get_commandline_input(env, &mini);
	free_env_lst(*env);
	return (mini.exit_status);
}

static int	create_shlvl_in_env(t_single_linked_node *env)
{
	char	*args[3];

	args[0] = "export";
	args[1] = "SHLVL=1";
	args[2] = NULL;
	if (export(args, &env))
		return (1);
	return (0);
}

static int	update_shell_level(t_single_linked_node *env)
{
	t_single_linked_node	*shlvl_node;
	t_env_var				*tmp;
	char					*curr_shlvl;
	int						i;

	shlvl_node = get_env_from_lst("SHLVL", env);
	if (!shlvl_node)
	{
		if (create_shlvl_in_env(env))
			return (1);
		return (0);
	}
	tmp = (t_env_var *)shlvl_node->content;
	i = 0;
	while (tmp->value[i])
	{
		if (!ft_isdigit(tmp->value[i++]))
			return (error("var SHLVL is not a number"), 0);
	}
	curr_shlvl = ft_itoa(ft_atoi(tmp->value) + 1);
	if (!curr_shlvl)
		return (perror("minishell: malloc new shlvl failed"), 1);
	free(tmp->value);
	tmp->value = curr_shlvl;
	return (0);
}
