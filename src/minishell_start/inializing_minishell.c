/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inializing_minishell.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 14:57:36 by bastalze          #+#    #+#             */
/*   Updated: 2026/08/19 15:36:13 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int						initializing_minishell(char **envp);
static int				update_shell_level(t_single_linked_node *env);
t_single_linked_node	*creating_fake_env(void);

int	initializing_minishell(char **envp)
{
	t_single_linked_node	*env;
	t_minishell				mini;

	ft_bzero(&mini, sizeof(t_minishell));
	if (!envp || !envp[0])
		env = default_env();
	else
		env = env_to_lst(envp);
	if (!env)
		return (1);
	if (update_shell_level(env))
		return (1);
//	testing_parsing(env);
	get_commandline_input(env, &mini);
//	free_env_lst(env);
	return (0);
}

static int	update_shell_level(t_single_linked_node *env)
{
	t_single_linked_node	*shlvl_node;
	t_env_var				*tmp;
	char					*curr_shlvl;
	int						prev_s_nr;
	int						i;

	shlvl_node = get_env_from_lst("SHLVL", env);
	if (!shlvl_node)
		return (error("shell level couldn't be updated"), 0);
	tmp = (t_env_var *)shlvl_node->content;
	i = 0;
	while (tmp->value[i])
	{
		if (!ft_isdigit(tmp->value[i]))
			return (error("var SHLVL is not a number"), 0);
		i++;
	}
	prev_s_nr = ft_atoi(tmp->value);
	curr_shlvl = ft_itoa(prev_s_nr + 1);
	if (!curr_shlvl)
		return (perror("minishell: malloc new shlvl failed"), 1);
	free(tmp->value);
	tmp->value = curr_shlvl;
	return (0);
}

t_single_linked_node	*creating_fake_env(void)
{
	t_env_var				*env_struct;

	env_struct = malloc(sizeof(t_env_var));
	env_struct->key = strdup("VAR");
	env_struct->value = strdup("Hamster cage");
	return (ft_single_lstnew(env_struct));
}