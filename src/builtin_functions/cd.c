/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 20:59:35 by nildruon          #+#    #+#             */
/*   Updated: 2026/06/03 23:54:52 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//TODO: change OLDPWD if it exits (bash doenst create one if you dont have it)
//TODO: change PWD when you cd
//TODO: integrate environment changes when using cd

static char	*get_home_env(t_single_linked_node	*envp)
{
	t_env_var	*tmp;
	char		*to_find = "HOME";
	size_t		key_len;

	while (envp)
	{
		tmp = (t_env_var	*)envp->content;
		key_len = ft_strlen(tmp->key);
		if(key_len == ft_strlen(to_find))
		{
			if(ft_strncmp(to_find, tmp->key, key_len) == 0)
				return(tmp->value);
		}
		envp = envp->next;
	}
	return (NULL);
}

static void cd_no_args(t_single_linked_node	*envp)
{
	char	*home_env;

	home_env = get_home_env(envp);
	if(home_env)
	{
		if(chdir(home_env) == -1)
		{
			ft_putendl_fd(strerror(errno),2);
			exit(1);
		}
	}
	else
	{
		ft_putendl_fd("minishell: cd: HOME not set", 2);
		exit(1);
	}
	exit(0);
}

void cd(char **input, t_single_linked_node	*envp)
{
	int i;

	i = 0;
	while (input[i])
		i++;
	if(i > 2)
	{
		ft_putendl_fd("minishell: cd: too many arguments",2);
		exit(1);
	}
	if(i == 1)
		cd_no_args(envp);
	if(ft_strlen(input[1]) == 0)
		exit(0);
	if(chdir(input[1]) == -1)
	{
		ft_putstr_fd("minishell: cd: ",2);
		ft_putendl_fd(strerror(errno),2);
		exit(1);
	}
	exit(0);
} 

