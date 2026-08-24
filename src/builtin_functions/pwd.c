/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:41:51 by username          #+#    #+#             */
/*   Updated: 2026/08/25 01:28:43 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	pwd(char **input)
{
	char	*cwd;
	int		i;

	i = 1;
	while (input[i] && input[i][0] != '-')
		i++;
	if (input[i] && i == 1 && input[i][0] == '-')
	{
		ft_putstr_fd("minishell: pwd: ", 2);
		ft_putstr_fd(input[i], 2);
		ft_putendl_fd(": invalid option\npwd: usage: pwd [no options]", 2);
		return (2);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (1);
	ft_putendl_fd(cwd, 1);
	free(cwd);
	return (0);
}
