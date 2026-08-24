/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:41:32 by username          #+#    #+#             */
/*   Updated: 2026/08/25 01:27:57 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_valid_flag(char *str, int *print_nl)
{
	int	len;
	int	i;

	len = ft_strlen(str);
	if (len <= 1)
		return (0);
	i = 0;
	if (str[i++] != '-')
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	*print_nl = 0;
	return (1);
}

int	echo(char **input)
{
	int	print_nl;
	int	i;

	print_nl = 1;
	i = 1;
	while (input[i] && is_valid_flag(input[i], &print_nl))
		i++;
	while (input[i])
	{
		ft_putstr_fd(input[i], 1);
		if (input[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (print_nl)
		ft_putchar_fd('\n', 1);
	return (0);
}
