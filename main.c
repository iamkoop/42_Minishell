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


static int	rl_signal_hook_ctrl_c(void)
{
	if (g_exit_status == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line ("", 0);
		rl_redisplay();
		rl_done = 1;
	}
	return (0);
}


int	main(int argc, char	**argv, char **envp)
{
	struct sigaction	c;

	ft_bzero(&c, sizeof(struct sigaction));
	c.sa_handler = handler_c;
	c.sa_flags = SA_RESTART;
	sigemptyset(&c.sa_mask);
	if (sigaction(SIGINT, &c, NULL))
		return (perror("minishell: SIGINT failed"), 1);
//	rl_catch_signals = 0;
	rl_signal_event_hook = rl_signal_hook_ctrl_c;
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
