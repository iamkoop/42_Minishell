/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 14:21:48 by nildruon          #+#    #+#             */
/*   Updated: 2026/07/29 14:39:54 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int input_redir(char	*file, t_redir_type type, t_minishell	*mini)
{
	if(mini->in > -1)
	{
		mini->prev_in = mini->in;
		close(mini->prev_in);
		mini->prev_in = -42;
	}
	if(type == HERE)
	{
		mini->in = open(file, O_RDONLY);
		if(mini->in == -1)
			return(perror("open fail in input_redirection"), 0);
		return(1);
	}
	mini->in = open(file, O_RDONLY);
	if(mini->in == -1)
		return(perror("open in input_redirection"), 0);
	return(1);
}

static int output_redir(char	*file, t_redir_type type,t_minishell	*mini)
{
	if(mini->out > -1)
	{
		mini->prev_out = mini->out;
		close(mini->prev_out);
		mini->prev_out = -42;
	}
	if(type == APPEND)
	{
		mini->out = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if(mini->out == -1)
			return(perror("open fail in output_redirection"), 0);
		return(1);
	}
	mini->out = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(mini->out == -1)
		return(perror("open fail in output_redirection"), 0);
	return(1);
}

static int execute_redirections(t_minishell	*mini)
{
	if(mini->in > -1)
	{
		mini->redir_in = dup2(mini->in, STDIN_FILENO);
		if(mini->redir_in == -1)
		{
			perror("final in redirection");
			return(0);
		}
		close(mini->in);
	}
	if(mini->out > -1)
	{
		mini->redir_out = dup2(mini->out, STDOUT_FILENO);
		if(mini->redir_out == -1)
		{
			perror("final out redirection");
			return(0);
		}
		close(mini->out);
	}
	return(1);
}

int redirections(t_single_linked_node	*redir_lst, t_minishell	*mini)
{
	t_redir_list	*curr_redir;

	while (redir_lst)
	{
		curr_redir = (t_redir_list	*)redir_lst->content;
		if(curr_redir->redir_type == IN || curr_redir->redir_type == HERE)
		{
			if(!input_redir(curr_redir->filename, curr_redir->redir_type ,mini))
				return(0);
		}
		if(curr_redir->redir_type == OUT || curr_redir->redir_type == APPEND)
		{
			if(!output_redir(curr_redir->filename, curr_redir->redir_type, mini))	
				return(0);
		}
		redir_lst = redir_lst->next;
	}
	return(execute_redirections(mini));
}
