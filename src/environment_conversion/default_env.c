/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   default_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 10:31:50 by bastalze          #+#    #+#             */
/*   Updated: 2026/08/17 12:33:03 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_single_linked_node	*default_env();
static int				add_to_list(t_single_linked_node **env, char *str);

t_single_linked_node	*default_env()
{
	char					*cwd;
	char					*pwd_as_var;
	t_single_linked_node	*env;

	env = NULL;
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (NULL);
	pwd_as_var = ft_strjoin("PWD=", cwd);
	free(cwd);
	if (!pwd_as_var)
		return (NULL);
	if (add_to_list(&env, pwd_as_var))
		return (free(pwd_as_var), NULL);
	free(pwd_as_var);
	if (add_to_list(&env, "SHLVL=1"))
		return (NULL);
	if (add_to_list(&env, "PATH=/usr/local/bin:/usr/bin:/bin"))
		return (NULL);
	return (env);
}

static int	add_to_list(t_single_linked_node **env, char *str)
{
	t_single_linked_node    *new_node;
	t_env_var				*content;

	content = create_env_node(str);
	if (!content)
	{
		return (perror("minishell: malloc for env list failed"),
				free_env_lst(*env), 1);
	}
	new_node = ft_single_lstnew(content);
	if (!new_node)
	{
		return (perror("minishell: malloc for env list failed"),
				free_env_lst(*env), 1);
	}
	if (*env)
		ft_lstadd_back_single_linked(env, new_node);
	else
		*env = new_node;
	return (0);
}
