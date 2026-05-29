/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 14:41:32 by nildruon          #+#    #+#             */
/*   Updated: 2026/05/29 15:56:44 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int is_validFlag(char	*str, int	*print_nl)
{
	int len;
	int i;

	len = ft_strlen(str);
	if(len <= 1)
		return(0);
	i = 0;
	if(str[i++] != '-')
		return(0);
	while (str[i])
	{
		if(str[i] != 'n')
			return(0);
		i++;
	}
	*print_nl = 0;
	return(1);
}
 
void echo(char	**input)
{
	int print_nl;
	int print;
	int i;

	print = 0;
	print_nl = 1;
	i = 1;
	while (input[i] && is_validFlag(input[i], &print_nl))
			i++;
	while (input[i])
	{
		ft_putstr_fd(input[i],1);
		if(input[i +1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	
	if(print_nl)
		ft_putchar_fd('\n', 1);
}
