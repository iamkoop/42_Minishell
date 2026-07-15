/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 14:21:48 by nildruon          #+#    #+#             */
/*   Updated: 2026/07/15 18:19:03 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//append goes to output and here doc to input

static void input_redirection(char	*file, t_minishell	*mini)
{
	if(mini->in > -1)
		mini->prev_in = mini->in;
}

static void output_redirection(char	*file, t_minishell	*mini)
{
	if(mini->out > -1)
		mini->prev_out = mini->out;
	mini->out = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(mini->out == -1)
	{
		perror("open in output_redirection");
	}
}

void redir_main(t_single_linked_node	*redir_lst, t_minishell	*mini)
{
	t_redir_list	*curr_redir;

	while (redir_lst)
	{
		if(mini->prev_in > -1)
			close(mini->prev_in);
		if(mini->prev_out > -1)
			close(mini->prev_out);
		curr_redir = (t_redir_list	*)redir_lst->content;
		if(curr_redir->redir_type == IN || HERE)
			redir_in(curr_redir->filename, mini);
		if(curr_redir->redir_type == OUT || APPEND)
			redir_out(curr_redir->filename, mini);
		redir_lst = redir_lst->next;
	}
}
