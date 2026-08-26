/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_all_fds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 18:04:21 by nilsdruon         #+#    #+#             */
/*   Updated: 2026/08/26 14:34:13 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void close_all_fds(t_minishell  *mini)
{
    close_fd(&mini->next_pipe_fds[0]);
    close_fd(&mini->next_pipe_fds[1]);
    close_fd(&mini->prev_read_fd);
    close_fd(&mini->redir_in);
    close_fd(&mini->redir_out);
    close_fd(&mini->in);
    close_fd(&mini->out);
    close_fd(&mini->prev_in);
    close_fd(&mini->prev_out);
}
