/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 20:59:35 by nildruon          #+#    #+#             */
/*   Updated: 2026/06/11 22:10:15 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//TODO: change OLDPWD if it exits (bash doenst create one if you dont have it)
//TODO: change PWD when you cd
//TODO: integrate environment changes when using cd

static char *get_home_env(t_single_linked_node	*envp)
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

static int cd_no_args(t_single_linked_node	*envp)
{
	char	*home_env;

	home_env = get_home_env(envp);
	if(home_env)
	{
		if(chdir(home_env) == -1)
		{
			ft_putendl_fd(strerror(errno),2);
			return (1);
		}
	}
	else
	{
		ft_putendl_fd("minishell: cd: HOME not set", 2);
		return (1);
	}
	return (0);
}

static void update_env(char	*pwd, char	*to_find, t_single_linked_node	*envp)
{
	t_env_var	*tmp;
	t_pwd_and_key_len	l;

	l.pwd_l = ft_strlen(to_find);
	while (envp != NULL)
	{
		tmp = (t_env_var	*)envp->content;
		l.key_l = ft_strlen(tmp->key);
		if (l.key_l == l.pwd_l && !ft_strncmp(to_find, tmp->key, l.key_l))
		{
			free(tmp->value);
			tmp->value = ft_strdup(pwd);
			if(!tmp->value)
				ft_putendl_fd("minishell: cd: malloc fail", 2);
			break ;
		}
		envp = envp->next;
	}
}

static int above_dir_del_case(t_pwds_vars *pwds, t_single_linked_node	*envp, char	*cmd_arg)
{
	char	*new_pwd;
	
	update_env(pwds->old_pwd, "OLDPWD", envp);
	new_pwd = ft_strjoin_three(pwds->old_pwd, "/", cmd_arg);
	if(!new_pwd)
		return(ft_putendl_fd("minishell: cd: malloc fail", 2), 1);
	update_env(new_pwd, "PWD",envp);
	free(new_pwd);
	perror("error retrieving current directory: getcwd");
	return(0);
}

static void copy_pwd_from_env(t_pwds_vars *pwds, char	*to_find, t_single_linked_node	*envp)
{
	t_env_var	*tmp;
	t_pwd_and_key_len	l;

	l.pwd_l = ft_strlen(to_find);
	pwds->old_pwd[0] = '\0';
	while (envp != NULL)
	{
		tmp = (t_env_var	*)envp->content;
		l.key_l = ft_strlen(tmp->key);
		if (l.key_l == l.pwd_l && !ft_strncmp(to_find, tmp->key, l.key_l))
		{
			ft_strlcpy(pwds->old_pwd, tmp->value, ft_strlen(tmp->value));
			break ;
		}
		envp = envp->next;
	}
	if (pwds->old_pwd[0] == '\0')
            ft_strlcpy(pwds->old_pwd, ".", sizeof(pwds->old_pwd));
}

static int dash_error_msg(char *msg ,int is_dash)
{
	if(is_dash)
		ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
	else
		perror(msg);
	return(1);
}

int cd(char **input, t_single_linked_node	*envp)
{
	t_pwds_vars pwds;
	int i;

	i = 0;
	pwds.is_dash = 0;
	while (input[i])
		i++;
	if(i > 2)
		return (ft_putendl_fd("minishell: cd: too many arguments", 2), 1);
	if(i == 1)
		return (cd_no_args(envp));
	if(ft_strlen(input[1]) == 0)
		return(0);
	if(ft_strlen(input[1]) == 1 && input[1][0] == '-')
		pwds.is_dash = 1;
	if(!getcwd(pwds.old_pwd, sizeof(pwds.old_pwd)))
		copy_pwd_from_env(&pwds, "PWD", envp);
	if(chdir(input[1]) == -1)
		return (dash_error_msg("minishell: cd ", pwds.is_dash));
	if(!getcwd(pwds.new_pwd, sizeof(pwds.new_pwd)))
		return(above_dir_del_case(&pwds, envp, input[1]));
	update_env(pwds.old_pwd, "OLDPWD", envp);
	update_env(pwds.new_pwd, "PWD", envp);
	return (0);
} 

