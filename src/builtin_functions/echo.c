/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:41:32 by nildruon          #+#    #+#             */
/*   Updated: 2026/05/27 19:11:05 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void print_input(char	**input, int start)
{
	while (input[start])
	{
		ft_putstr_fd(input[start],1);
		if(input[start + 1])
			ft_putchar_fd(' ', 1);
		start++;
	}
	
}

//we are asumming that the input is echo word1/flag word2 word3 etc..
//TODO: Handle -nnnn and check with peers what they did with the other flags..
void echo(char	**input)
{
	int len;

	len = 0;
	while (input[len])
		len++;
	if(len == 1)
	{
		ft_putchar_fd('\n',1);
		return ;
	}
	if(ft_strlen(input[1]) == 2 && ft_strncmp(input[1], "-", 1) == 0)
	{
		if(len == 2)
			return ;
		print_input(input, 2);
	}
	else
	{
		print_input(input, 1);
		ft_putchar_fd('\n', 1);
	}
}