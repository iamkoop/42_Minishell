/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_heredoc_fds.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 16:36:21 by bastalze          #+#    #+#             */
/*   Updated: 2026/08/20 21:07:04 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void    close_heredoc_fds(t_minishell *mini)
{
    t_token_iteri	iteri;

	ft_bzero(&iteri, sizeof(t_token_iteri));
    iteri.tok = (t_token_node *)mini->arena_tokens.data;
    while (iteri.tok->token_type)
	{
        if (iteri.tok->token_type == HERE_DOC
                && (iteri.tok + 1)->token_type
                && (iteri.tok + 1)->token_type == WORD)
        {
            if ((iteri.tok + 1)->hd_fd)
            {
                close((iteri.tok + 1)->hd_fd);
                (iteri.tok + 1)->hd_fd = 0;
            }
        }
        iteri.tok++;
	}
}