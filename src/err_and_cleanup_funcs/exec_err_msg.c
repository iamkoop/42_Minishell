/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_err_msg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 12:37:36 by nildruon          #+#    #+#             */
/*   Updated: 2026/08/25 14:22:58 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	err_msg(char	*func, char *value, char	*custom_err)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(func, 2);
	ft_putstr_fd(": ", 2);
	if(value)
	{
		ft_putstr_fd(value, 2);
		ft_putstr_fd(": ", 2);	
	}
	if(!custom_err)
		ft_putendl_fd(strerror(errno), 2);
	else
		ft_putendl_fd(custom_err, 2);
}
