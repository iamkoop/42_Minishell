/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 11:51:03 by nildruon          #+#    #+#             */
/*   Updated: 2026/08/17 14:54:22 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "testing.h"

volatile sig_atomic_t	g_signal_code = 0;

static void	handler_c(int signo)
{
	g_signal_code = signo;
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
	g_signal_code = 0;
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
	/*
	if (!*envp)
	{
		if (default_environment())
			return (1);
	}
	*/	
	if (initializing_minishell(envp))
		return (1);
	printf("Testing Minishell\n");
	printf("---------------------------\n");
	printf("========================================\n");
	printf("|          ALL EXECUTION TEST          |\n");
	printf("========================================\n");
	testing_exec(argv, envp);
	printf("\n");
	printf("========================================\n");
	printf("|          ALL PARSING TESTS           |\n");
	printf("========================================\n");
//	t_single_linked_node *env = creating_fake_env();
//	testing_parsing(env);
//	rl_clear_history();
}