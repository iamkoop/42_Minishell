/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 08:17:58 by nilsdruon         #+#    #+#             */
/*   Updated: 2026/07/05 01:04:29 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int execute_builtin(char   **cmd_and_args, t_single_linked_node  *envp)
{
    if (ft_strncmp(cmd_and_args[0], "cd", ft_get_biggest_s(cmd_and_args, "cd")) == 0)
		return (cd(cmd_and_args, envp));
    if (ft_strncmp(cmd_and_args[0], "echo", ft_get_biggest_s(cmd_and_args, "echo")) == 0)
		return (echo(cmd_and_args));
    if (ft_strncmp(cmd_and_args[0], "env", ft_get_biggest_s(cmd_and_args, "env")) == 0)
		return (env(cmd_and_args, envp));
    if (ft_strncmp(cmd_and_args[0], "exit", ft_get_biggest_s(cmd_and_args, "exit")) == 0)
		builtin_exit(cmd_and_args);
    if (ft_strncmp(cmd_and_args[0], "export", ft_get_biggest_s(cmd_and_args, "export")) == 0)
		return (export(cmd_and_args, envp));
    if (ft_strncmp(cmd_and_args[0], "pwd", ft_get_biggest_s(cmd_and_args, "pwd")) == 0)
		return (pwd(cmd_and_args));
    if (ft_strncmp(cmd_and_args[0], "unset", ft_get_biggest_s(cmd_and_args, "unset")) == 0)
		return (unset(cmd_and_args, &envp));
	return (0);
}

int exec_command(char   **cmd_and_args, t_single_linked_node    *envp)
{

    if(is_builtin(cmd_and_args[0]))
        return(execute_builtin(cmd_and_args, envp));
    if()
    return(1);
}
