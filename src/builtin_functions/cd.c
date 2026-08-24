/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 20:59:35 by nildruon          #+#    #+#             */
/*   Updated: 2026/08/25 01:04:51 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_env_var *get_env(t_single_linked_node	*envp, char		*to_find)
{
	t_env_var	*tmp;

	while (envp)
	{
		tmp = (t_env_var	*)envp->content;
		if(ft_strcmp(to_find, tmp->key) == 0)
				return(tmp);
		envp = envp->next;
	}
	return (NULL);
}

static int update_env(char	*pwd, char	*to_find, t_single_linked_node	*envp)
{
	t_single_linked_node	*new_node;
	t_env_var	*tmp;
	char		*new_env_var;

	tmp = get_env(envp, to_find);
	if(!tmp)
	{
		new_env_var = ft_strjoin_three(to_find,"=",pwd);
		if(!new_env_var)
			return (0);
		tmp = create_env_node(new_env_var);
		if(!tmp)
			return (free(new_env_var), 0);
		new_node = ft_single_lstnew(tmp);
		if(!new_node)
			return (free(new_env_var), del_env_node_content(tmp), 0);
		ft_lstadd_back_single_linked(&envp, new_node);
		return(1);
	}
	free(tmp->value);
	tmp->value = ft_strdup(pwd);
	if(!tmp->value)
		return(0);
	return(1);
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

	pwds->old_pwd[0] = '\0';
	tmp = get_env(envp, to_find);
	if(tmp)
		ft_strlcpy(pwds->old_pwd, tmp->value, sizeof(pwds->old_pwd));
	if (pwds->old_pwd[0] == '\0')
            ft_strlcpy(pwds->old_pwd, ".", sizeof(pwds->old_pwd));
}


static char *find_target(char	**input, t_single_linked_node	*envp)
{
	t_env_var	*tmp;

	if(input[0] && !input[1])
	{
		tmp = get_env(envp, "HOME");
		if(!tmp)
			return(ft_putendl_fd("minishell: cd: HOME not set", 2), NULL);
		return(tmp->value);
	}
	if(!ft_strcmp(input[1], "-"))
	{
		tmp = get_env(envp, "OLDPWD");
		if(!tmp)
			return(ft_putendl_fd("minishell: cd: OLDPWD not set", 2), NULL);
		return(tmp->value);
	}	
	return(input[1]);
}

void err_msg(char	*target)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(target, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errno), 2);
}

int cd(char **input, t_single_linked_node	*envp)
{
	char		*target;
	t_pwds_vars pwds;

	if(input[0] && input[1] && input[2])
		return (ft_putendl_fd("minishell: cd: too many arguments", 2), 1);
	if(input[1] && ft_strlen(input[1]) == 0)
		return(0);
	target = find_target(input, envp);
	if(!target)
		return(1);
	if(!getcwd(pwds.old_pwd, sizeof(pwds.old_pwd)))
		copy_pwd_from_env(&pwds, "PWD", envp);
	if(chdir(target) == -1)
		return (err_msg(target), 1);
	if(!getcwd(pwds.new_pwd, sizeof(pwds.new_pwd)))
		return(above_dir_del_case(&pwds, envp, input[1]));
	if(input[1] && !ft_strcmp(input[1], "-"))
		printf("%s\n", pwds.new_pwd);
	if(!update_env(pwds.old_pwd, "OLDPWD", envp))
		ft_putendl_fd("minishell: cd: malloc fail in update_env", 2);
	if(!update_env(pwds.new_pwd, "PWD", envp))
		ft_putendl_fd("minishell: cd: malloc fail in update_env", 2);
	return (0);
} 

