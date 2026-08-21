/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 08:17:58 by nilsdruon         #+#    #+#             */
/*   Updated: 2026/08/21 18:53:40 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int execute_builtin(char   **cmd_and_args, t_single_linked_node  **envp)
{
	if (ft_strncmp(cmd_and_args[0], "cd", ft_get_biggest_s(cmd_and_args[0], "cd")) == 0)
		return (cd(cmd_and_args, *envp));
	if (ft_strncmp(cmd_and_args[0], "echo", ft_get_biggest_s(cmd_and_args[0], "echo")) == 0)
	  	return (echo(cmd_and_args));
	if (ft_strncmp(cmd_and_args[0], "env", ft_get_biggest_s(cmd_and_args[0], "env")) == 0)
			return (env(cmd_and_args, envp));
    if (ft_strncmp(cmd_and_args[0], "exit", ft_get_biggest_s(cmd_and_args[0], "exit")) == 0)
		builtin_exit(cmd_and_args);
    if (ft_strncmp(cmd_and_args[0], "export", ft_get_biggest_s(cmd_and_args[0], "export")) == 0)
		return (export(cmd_and_args, envp));
    if (ft_strncmp(cmd_and_args[0], "pwd", ft_get_biggest_s(cmd_and_args[0], "pwd")) == 0)
		return (pwd(cmd_and_args));
    if (ft_strncmp(cmd_and_args[0], "unset", ft_get_biggest_s(cmd_and_args[0], "unset")) == 0)
		return (unset(cmd_and_args, envp));
	return (1);
}

// TODO: copy argv and free then the whole cmdlst!!!!

void exec_command(char   **cmd_and_args, t_single_linked_node    **envp, t_minishell *mini)
{
    char            **converted_envp;
	char    *path;

	if(!cmd_and_args)
		return ;
    g_exit_status = 1;
    if(is_builtin(cmd_and_args[0]))
    {
      g_exit_status = execute_builtin(cmd_and_args, envp);
      return ;
    }
    path = get_path(cmd_and_args[0], *envp, mini);
    if(!path)
    {
		return ;
	}
    converted_envp = env_to_char_arr(*envp);
    if(!converted_envp)
        ft_putendl_fd("minishell: exec_command: conversion failed", 2);
    else
        execve(path, cmd_and_args, converted_envp);
    g_exit_status = 1;
}
