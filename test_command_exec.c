/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_command_exec.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 18:26:08 by nildruon          #+#    #+#             */
/*   Updated: 2026/05/06 17:08:57 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char	**argv, char	**envp)
{
	t_env_var	*env;
	
	if(argc != 2)
	{
		ft_putstr_fd("Wrong amount of arguments", 2);
		return(1);
	}
	
	argv = NULL;
}