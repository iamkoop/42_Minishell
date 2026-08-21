/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 11:51:03 by nildruon          #+#    #+#             */
/*   Updated: 2026/08/21 19:02:52 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_exit_status = 0;

static void	handler_c(int signo)
{
	g_exit_status = signo;
}

void	signal_ctrl_backslash(void)
{
	__sighandler_t	sig_return;

	sig_return = signal(SIGQUIT, SIG_IGN);
	if (sig_return == SIG_ERR)
		perror("minishell: error in signal when ignoring ctrl+\\");
}

static int	signal_ctrl_c(void)
{
//  need to set exit code to 130
	g_exit_status = 0;
	rl_on_new_line();
	rl_replace_line (NULL, 1);
	rl_redisplay();
	return (0);
}

int	main(int argc, char	**argv, char **envp)
{
	struct sigaction	c;

	c.sa_handler = &handler_c;
	sigemptyset(&c.sa_mask);
	if (sigaction(SIGINT, &c, NULL))
		return (perror("minishell: SIGINT failed"), 1);
	rl_signal_event_hook = &signal_ctrl_c;
	signal_ctrl_backslash();
	if (argc != 1)
		return (write(2, "minishell: program takes no arguments", 37), 1);
	if (argv[0])
		argv = NULL;
	//main_testing(argv, envp);
	if (initializing_minishell(envp))
		return (1);
	rl_clear_history();
	return (g_exit_status);
}
