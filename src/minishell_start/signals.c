/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 17:11:31 by nildruon          #+#    #+#             */
/*   Updated: 2026/09/07 18:01:52 by nildruon         ###   ########.fr       */
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

void	ignore_sigquit(void)
{
	__sighandler_t	sig_return;

	sig_return = signal(SIGQUIT, SIG_IGN);
	if (sig_return == SIG_ERR)
		perror("minishell: error in signal when ignoring SIGQUIT");
}

static void	handler_c(int signo)
{
	g_signal = signo;
}

static int	rl_signal_hook_ctrl_c(void)
{
	if (g_signal == SIGINT)
	{
		//write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_redisplay();
		//ioctl(1, '\n');
		return (1);
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
