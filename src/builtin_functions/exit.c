/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:41:45 by username          #+#    #+#             */
/*   Updated: 2026/09/01 14:54:45 by nildruon         ###   ########.fr       */
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
	ll_min = "9223372036854775808";
	i = -1;
	while (*str && *str == '0')
		str++;
	if(!*str)
		return(1);
	len = ft_strlen(str);
	if(len < 19)
		return (1);
	while (str[++i])
	{
		if(!ft_isdigit(str[i]))
			break ;
		if ((is_negative && (str[i] > ll_min[i])) ||
			(!is_negative && (str[i] > ll_max[i])))
			return (0);
	}
	return (1);
}

static int	num_is_valid(char *str)
{
	int	i;
	int	is_negative;

	i = 0;
	while (*str && *str == ' ')
		str++;
	if (str[i] == '-' || str[i] == '+' || ft_isdigit(str[i]))
		i++;
	if(i == 0 || !str[i] || !ft_isdigit(str[i]))
		return(0);
	while (str[i] && ft_isdigit(str[i]))
		i++;
	while (str[i])
	{
		if(str[i] != ' ')
			return (0);
		i++;
	}
	if (str[0] == '-')
		is_negative = 1;
	else
		is_negative = 0;
	if(str[0] == '+' || str[0] == '-')
		str++;
	return (num_is_valid_help(str, is_negative));
}

static void	exit_error_msg(char *arg)
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

void	builtin_exit(char **input, t_single_linked_node	**envp, t_minishell	*mini)
{
	int	len;
	long long	exit_code;

	exit_code = -42;
	if (!input[1])
		exit_code = 0;
	len = 1;
	while (input[len])
	{
		if (len > 1)
		{
			exit_error_msg(NULL);
			return ;
		}
		if (!num_is_valid(input[len]))
		{
			exit_error_msg(input[len]);
			exit_code = 2;
			break;
		}
		len++;
	}
	if(exit_code == -42)
		exit_code = ft_atoll(input[1]);
	free_command_struct(mini->cmd_lst);
	ft_single_lstclear(envp, del_env_node_content);
	rl_clear_history();
	ft_putendl_fd("exit", 1);
	exit((unsigned char)exit_code);
}
