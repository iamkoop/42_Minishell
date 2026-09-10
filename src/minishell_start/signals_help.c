/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_help.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 19:13:26 by nildruon          #+#    #+#             */
/*   Updated: 2026/09/10 10:10:35 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
