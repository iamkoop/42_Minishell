/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 11:51:03 by username          #+#    #+#             */
/*   Updated: 2026/09/07 17:41:05 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

void	main_testing(char **argv, char **envp);

int	main(int argc, char **argv, char **envp)
{
	int					exit_status;

	if (argc != 1)
		return (write(2, "minishell: program takes no arguments\n", 38), 1);
	if (argv[0])
		argv = NULL;
	signal_strl_c();
	ignore_sigquit();
	exit_status = initializing_minishell(envp);
	rl_clear_history();
	return (exit_status);
}
