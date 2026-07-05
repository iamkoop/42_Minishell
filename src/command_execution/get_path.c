/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 15:22:21 by nilsdruon         #+#    #+#             */
/*   Updated: 2026/07/05 11:33:55 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*check_access(char	*cmd, int *exit_status, int send_perror)
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
				perror(cmd);
			if (!*exit_status)
				*exit_status = 126;
		}
	}
	else
	{
		if (send_perror)
			perror(cmd);
		if (!*exit_status)
			*exit_status = 127;
	}
	return (NULL);
}

static char	*colon_edge_case(char *env, int	*exit_status)
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
				return (malloc_fail_handler("join failed\n", exit_status, 0));
			if (check_access(joined, 0, 0))
				return (joined);
		}
		i++;
	}
	if (joined)
		free(joined);
	return (NULL);
}

static char	*find_exacutable(char *cmd, char *env, int	*exit_status)
{
	char	*command;
	char	**splitted;
	int		i;

	command = colon_edge_case(env, exit_status);
	if (command)
		return (command);
	splitted = ft_split(env, ':');
	if (!splitted)
		return (ft_putstr_fd("split alloc fail", 2), NULL);
	i = 0;
	while (splitted[i])
	{
		command = ft_strjoin_three(splitted[i], "/", cmd);
		if (!command)
			return (ft_putstr_fd("cmd alloc fail\n", 2),
				free_the_split(splitted), NULL);
		if (check_access(command, exit_status, 0))
			return (free_the_split(splitted), command);
		free(command);
		i++;
	}
	free_the_split(splitted);
	return (perror(cmd), NULL);
}

static char	*check_if_its_a_path(char *cmd, int	*exit_status)
{
	if (!cmd || !*cmd)
	{
		*exit_status = 127;
		return (NULL);
	}
	if (ft_strchr(cmd, '/'))
	{
		if (check_access(cmd, exit_status, 1))
			return (cmd);
	}
	return (NULL);
}

char	*get_path(t_pipex *pip, char *cmd)
{
	int		i;
	char	*path;

	i = 0;
	if (!cmd || !*cmd)
		return (get_path_err_handler(pip, cmd, 0), NULL);
	if (check_if_its_a_path(cmd, &pip->exit_status))
		return (ft_strdup(cmd));
	while (pip->envp && pip->envp[i])
	{
		if (ft_strncmp(pip->envp[i], "PATH=", 5) == 0)
		{
			path = find_exacutable(cmd, pip->envp[i] + 5, &pip->exit_status);
			if (!path || !*path)
			{
				if (!pip->exit_status)
					pip->exit_status = 127;
				return (NULL);
			}
			return (path);
		}
		i++;
	}
	get_path_err_handler(pip, cmd, 1);
	return (NULL);
}

char	*get_path(char *cmd, t_single_linked_node   *envp)
{
    if (!cmd || !*cmd)
    {
        
    }
}