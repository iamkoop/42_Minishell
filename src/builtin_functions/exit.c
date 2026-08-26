/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:41:45 by username          #+#    #+#             */
/*   Updated: 2026/08/25 01:35:58 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	num_is_valid_help(char *str, int is_negative)
{
	char	*ll_max;
	char	*ll_min;
	int		i;
	int		len;

	ll_max = "9223372036854775807";
	ll_min = "-9223372036854775808";
	i = -1;
	if (is_negative)
		i++;
	len = ft_strlen(str);
	if ((!is_negative && len < 19) || (is_negative && len < 20))
		return (1);
	while (str[++i])
	{
		if (is_negative && (str[i] > ll_min[i]))
			return (0);
		if (!is_negative && (str[i] > ll_max[i]))
			return (0);
	}
	return (1);
}

static int	num_is_valid(char *str)
{
	int	i;
	int	is_negative;

	i = 0;
	if (str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isalnum(str[i++]))
			return (0);
	}
	if (str[0] == '-')
		is_negative = 1;
	else
		is_negative = 0;
	return (num_is_valid_help(str, is_negative));
}

static void	error_msg(char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		ft_putendl_fd(": numeric argument required", 2);
	}
	else
		ft_putendl_fd("too many arguments", 2);
}

void	builtin_exit(char **input)
{
	int	len;

	len = 0;
	ft_putendl_fd("exit", 1);
	if (!input[1])
		exit(0);
	len = 1;
	while (input[len])
	{
		if (len > 1)
		{
			error_msg(NULL);
		}
		if (!num_is_valid(input[len]))
		{
			error_msg(input[len]);
			exit(2);
		}
		len++;
	}
	exit((unsigned char) ft_atoll(input[1]));
}
