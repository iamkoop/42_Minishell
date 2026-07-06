/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 15:22:21 by nilsdruon         #+#    #+#             */
/*   Updated: 2026/07/06 17:21:59 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void print_error(char	*cmd)
{
	ft_putstr_fd("minishell: ", 2);
	perror(cmd);
}

static char	*check_access(char	*cmd, t_exit_status	*mini, int send_perror)
{
	int	access_valid;

	access_valid = access(cmd, F_OK);
	if (!access_valid)
	{
		access_valid = access(cmd, X_OK);
		if (!access_valid)
			return (cmd);
		else
		{
			if (send_perror)
				print_error(cmd);
			mini->exit_status = 126;
		}
	}
	else
	{
		if (send_perror)
			print_error(cmd);
		mini->exit_status = 127;
	}
	return (NULL);
}

static char	*colon_edge_case(char *env, t_exit_status	*mini)
{
	int		i;
	int		len;
	char	*joined;

	i = 0;
	len = ft_strlen(env);
	joined = NULL;
	while (env[i])
	{
		if (env[0] == ':' || env[len -1] == ':'
			|| (env[i + 1] && ft_strncmp(&env[i], "::", 2) == 0))
		{
			joined = ft_strjoin("./", env);
			if (!joined)
			{
				ft_putendl_fd("minishell: colon_edge_case: join failed", 2);
				mini->exit_status = 1;
				return(NULL);
			}
			if (check_access(joined, 0, 0))
				return (joined);
		}
		i++;
	}
	if (joined)
		free(joined);
	return (NULL);
}

static char	*find_exacutable(char *cmd, char *env, t_exit_status	*mini)
{
	char	*command;
	char	**splitted;
	int		i;

	command = colon_edge_case(env, mini);
	if (command)
		return (command);
	splitted = ft_split(env, ':');
	if (!splitted)
		return (ft_putendl_fd("minishell: find_exacutable: split alloc fail", 2), NULL);
	i = 0;
	while (splitted[i])
	{
		command = ft_strjoin_three(splitted[i], "/", cmd);
		if (!command)
			return (ft_putstr_fd("cmd alloc fail\n", 2),
				ft_free_the_split(splitted), NULL);
		if (check_access(command, mini, 0))
			return (ft_free_the_split(splitted), command);
		free(command);
		i++;
	}
	ft_free_the_split(splitted);
	return (print_error(cmd), NULL);
}

static char	*check_if_its_a_path(char *cmd, t_exit_status *mini, int *iterate)
{
	if (ft_strchr(cmd, '/'))
	{
		if (check_access(cmd, mini, 1))
		{
			*iterate = 1;
			return (cmd);
		}
		else
			*iterate = 0;
	}
	return (NULL);
}

char	*get_path(char *cmd, t_single_linked_node   *envp, t_exit_status *mini)
{
	t_env_var	*content;
	char		*path;
	int			iterate_envp;
	
    if (!cmd || !*cmd)
    {
		mini->exit_status = 127;
		ft_putstr_fd("Minishell: command not found\n", 2);
	}
	if(check_if_its_a_path(cmd, mini, &iterate_envp))
		return(ft_strdup(cmd));
	while (envp && iterate_envp)
	{
		content = (t_env_var	*)envp->content;
		if (ft_strncmp(content->key, "PATH", 4) == 0)
		{
			path = find_exacutable(cmd, content->value, mini);
			if (!path || !*path)
				return (NULL);
			return (path);
		}
		envp = envp->next;
	}
	mini->exit_status = 127;
	return (NULL);
}
