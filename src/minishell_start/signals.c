/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 17:11:31 by nildruon          #+#    #+#             */
/*   Updated: 2026/09/08 16:51:28 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	set_sigquit_to_default(void)
{
	__sighandler_t	sig_return;

	sig_return = signal(SIGQUIT, SIG_DFL);
	if (sig_return == SIG_ERR)
		perror("minishell: error in signal when setting SIGQUIT to default");
}

void	set_sigint_to_default(void)
{
	__sighandler_t	sig_return;

	sig_return = signal(SIGINT, SIG_DFL);
	if (sig_return == SIG_ERR)
		perror("minishell: error in signal when setting SIGINT to default");
}

void	ignore_sigint(void)
{
	__sighandler_t	sig_return;

	sig_return = signal(SIGINT, SIG_IGN);
	if (sig_return == SIG_ERR)
		perror("minishell: error in signal when ignoring SIGINT");
}

static void	handler_quit(int sigquit)
{
	(void)sigquit;
}

void	ignore_sigquit(void)
{
	struct sigaction	quit;

	ft_bzero(&quit, sizeof(struct sigaction));
	quit.sa_handler = handler_quit;
	quit.sa_handler = SIG_IGN;
	sigemptyset(&quit.sa_mask);
	if (sigaction(SIGQUIT, &quit, NULL))
		perror("minishell: SIGQUIT failed");
	/*
	__sighandler_t	sig_return;

	sig_return = signal(SIGQUIT, SIG_IGN);
	if (sig_return == SIG_ERR)
		perror("minishell: error in signal when ignoring SIGQUIT");
	*/
}

static void	handler_c(int signo)
{
	g_signal = signo;
}

static int	rl_signal_hook_ctrl_c(void)
{
	if (g_signal == SIGINT)
	{
		/*
		//write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_redisplay();
		//ioctl(1, '\n');
		return (1);
	*/
		if (ioctl(STDIN_FILENO, TIOCSTI, "\n") == -1)
			error("ioctl error");
		rl_replace_line("", 0);
		return (0);
	}
	return (0);
}

void	signal_strl_c(void)
{
	struct sigaction	c;

	ft_bzero(&c, sizeof(struct sigaction));
	c.sa_handler = handler_c;
	sigemptyset(&c.sa_mask);
	if (sigaction(SIGINT, &c, NULL))
		perror("minishell: SIGINT failed");
	else
		rl_signal_event_hook = rl_signal_hook_ctrl_c;
}
