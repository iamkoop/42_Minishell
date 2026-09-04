/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 15:22:21 by nilsdruon         #+#    #+#             */
/*   Updated: 2026/09/04 17:55:49 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


static char	*check_access(char	*cmd, t_minishell	*mini, int send_perror)
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
			{
				ft_putstr_fd("minishell: ", 2);
				perror(cmd);
			}
			mini->exit_status = 126;
		}
	}
	else
	{
		if (send_perror)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(cmd);
		}
		mini->exit_status = 127;
	}
	return (NULL);
}

static char	*colons(char	*path_var,char	*cmd, int	*to_move, t_minishell	*mini)
{
	char	*full_cmd;
	int middle;
	int i;

	i = *to_move;
	*to_move = 0;
	while (path_var[*to_move] && path_var[*to_move] == ':')
		(*to_move)++;
	middle = 1;
	if(!path_var[*to_move] || i == -1337)
		middle = 0;
	i = 0;
	while ((!middle && i < *to_move) || (middle && i < *to_move -1))
	{
		full_cmd = ft_strjoin("./", cmd);
		if(!full_cmd)
		{
			ft_putendl_fd("minishell: malloc fail in colons", 2);
			return (*to_move = -42, NULL);
		}
		if(check_access(full_cmd, mini, 0))
			return(full_cmd);
		free(full_cmd);
		i++;
	}
	return(NULL);
}

static char	*normal_dir(char	*path_var, char	*cmd, int	*to_move, t_minishell	*mini)
{
	char	*cmd_path;
	char	*full_cmd;

	*to_move = 0;
	while (path_var[*to_move] && path_var[*to_move] != ':')
		(*to_move)++;
	cmd_path = ft_calloc(*to_move + 1, sizeof(char));
	if(!cmd_path)
	{
		ft_putendl_fd("minishell: malloc fail in normal_dir", 2);
		return (*to_move = -42, NULL);
	}
	ft_strlcpy(cmd_path, path_var, *to_move + 1);
	full_cmd = ft_strjoin_three(cmd_path, "/", cmd);
	if(!full_cmd)
	{
		ft_putendl_fd("minishell: malloc fail in normal_dir", 2);
		return (*to_move = -42, NULL);
	}
	if(check_access(full_cmd, mini, 0))
			return(full_cmd);
	free(full_cmd);
	return(NULL);
}

static char	*find_exacutable(char *path_var, char	*cmd, t_minishell	*mini)
{
	char	*full_cmd;
	int		to_move;

	to_move = -1337;
	if(!ft_strlen(path_var))
		return (err_msg(NULL, cmd, "No such file or directory"), NULL);
	while (*path_var)
	{
		if(to_move != -1337)
			to_move = 0;
		if(*path_var == ':')
			full_cmd = colons(path_var, cmd, &to_move, mini);
		else
			full_cmd = normal_dir(path_var, cmd, &to_move, mini);
		if(to_move == -42)
			return(NULL);
		if(full_cmd)
			return(full_cmd);
		while (to_move-- > 0)
			path_var++;
	}
	return (err_msg(NULL, cmd, "command not found"), NULL);
}

char	*get_path(char *cmd, t_single_linked_node   *envp, t_minishell *mini)
{
	struct stat stats;
	t_env_var	*content;
	char		*path;
	
    mini->exit_status = 127;
	if (!cmd || !*cmd)
		return(err_msg(NULL, cmd, "command not found"), NULL);
	path = ft_strdup(cmd);
	if(!path)
		return (ft_putendl_fd("minishell: malloc fail in get_path", 2), NULL);
	if(ft_strchr(cmd, '/') && check_access(cmd, mini, 1))
	{
		if(stat(cmd, &stats) == 0)
		{
			if(S_ISDIR(stats.st_mode))
				return(err_msg(NULL, cmd, "Is a directory"), mini->exit_status = 126, NULL);
		}
		else
			return(free(path), perror("minishell: stat func failed"), NULL);
		return(path);
	}
	free(path);
	envp = get_env_from_lst("PATH", envp);
	if(!envp)
		return(err_msg(NULL, cmd, "No such file or directory"), NULL);
	content = (t_env_var	*)envp->content;
	path = find_exacutable(content->value, cmd, mini);
	if (!path || !*path)
		return(NULL);
	return (path);
}
