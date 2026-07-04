/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 10:22:12 by nilsdruon         #+#    #+#             */
/*   Updated: 2026/07/04 16:32:10 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, cd, ft_get_biggest_s(cmd, cd)) == 0
		|| ft_strncmp(cmd, "echo", ft_get_biggest_s(cmd, "echo")) == 0
		|| ft_strncmp(cmd, "env", ft_get_biggest_s(cmd, "env")) == 0
		|| ft_strncmp(cmd, "exit", ft_get_biggest_s(cmd, "exit")) == 0
		|| ft_strncmp(cmd, "export", ft_get_biggest_s(cmd, "export")) == 0
		|| ft_strncmp(cmd, "pwd", ft_get_biggest_s(cmd, "pwd")) == 0
		|| ft_strncmp(cmd, "unset", ft_get_biggest_s(cmd, "unset")) == 0)
		return (1);
	return (0);
}
