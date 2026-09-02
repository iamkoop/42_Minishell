/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 11:51:03 by username          #+#    #+#             */
/*   Updated: 2026/08/31 16:33:16 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	main_testing(char **argv, char **envp);
volatile sig_atomic_t	g_signal = 0;

static void	handler_c(int signo)
{
	g_signal = signo;
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
	if (g_signal == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	struct sigaction	c;
	int					exit_status;

	ft_bzero(&c, sizeof(struct sigaction));
	c.sa_handler = handler_c;
	c.sa_flags = SA_RESTART;
	sigemptyset(&c.sa_mask);
	if (sigaction(SIGINT, &c, NULL))
		return (perror("minishell: SIGINT failed"), 1);
	//	rl_catch_signals = 0;
	rl_signal_event_hook = rl_signal_hook_ctrl_c;
	/*if (argv && argv[0] && ft_strnstr(argv[0], "test", ft_strlen(argv[0])))
	{
		(void) exit_status;
		(void) argc;
		main_testing(argv, envp);
		return (0);
	}*/
	if (argc != 1)
		return (write(2, "minishell: program takes no arguments\n", 38), 1);
	if (argv[0])
		argv = NULL;
	signal_ctrl_backslash();
	exit_status = initializing_minishell(envp);
	rl_clear_history();
	return (exit_status);
}
