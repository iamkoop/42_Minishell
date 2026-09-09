/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 15:22:21 by nilsdruon         #+#    #+#             */
/*   Updated: 2026/09/09 19:37:15 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_path(char *cmd, t_single_linked_node	*envp, t_minishell	*mini)
{
	t_env_var	*content;
	char		*path;

	mini->exit_status = 127;
	if (!cmd || !*cmd)
		return (err_msg(NULL, cmd, "command not found"), NULL);
	path = ft_strdup(cmd);
	if (!path)
		return (ft_putendl_fd("minishell: malloc fail in get_path", 2), NULL);
	if (ft_strchr(path, '/'))
		return (path);
	free(path);
	envp = get_env_from_lst("PATH", envp);
	if (!envp)
		return (ft_strjoin_three(".", "/", cmd));
	content = (t_env_var *)envp->content;
	path = find_exacutable(content->value, cmd, mini);
	if (!path || !*path)
		return (NULL);
	return (path);
}
