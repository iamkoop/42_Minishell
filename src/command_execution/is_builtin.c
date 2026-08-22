/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 10:22:12 by nilsdruon         #+#    #+#             */
/*   Updated: 2026/08/21 19:09:19 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "cd") == 0
	|| ft_strcmp(cmd, "echo") == 0
	|| ft_strcmp(cmd, "env") == 0
	|| ft_strcmp(cmd, "export") == 0
	|| ft_strcmp(cmd, "exit") == 0
	|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "unset") == 0)
		return (1);
	return (0);
}
