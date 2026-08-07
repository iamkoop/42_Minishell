/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 14:21:48 by nildruon          #+#    #+#             */
/*   Updated: 2026/08/07 20:46:49 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int input_redir(char	*file, t_redir_type type, t_minishell	*mini)
{
	if(mini->in > -1)
	{
		mini->prev_in = mini->in;
		close_fd(&mini->prev_in);
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
		close_fd(&mini->prev_out);
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
		close_fd(&mini->in);
	}
	if(mini->out > -1)
	{
		mini->redir_out = dup2(mini->out, STDOUT_FILENO);
		if(mini->redir_out == -1)
		{
			perror("final out redirection");
			return(0);
		}
		close_fd(&mini->out);
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

int builtin_redir_special_case(t_minishell	*mini, t_single_linked_node	*envp)
{
	int		saved_in;
	int		saved_out;

	saved_in = dup(STDIN_FILENO);
	if(saved_in == -1)
		return(perror("dup failed in builtin edge_case"), 0);
	saved_out = dup(STDOUT_FILENO);
	if(saved_out == -1)
		return(perror("dup failed in builtin edge_case"), close(saved_in), 0);
	if(!redirections(mini->curr_cmd->redir, mini))
				return(close(saved_in),close(saved_out), 0);
	mini->exit_status = exec_command(mini->curr_cmd->argv, envp);
	if(dup2(saved_in, STDIN_FILENO) == -1)
		perror("dup failed in builtin edge_case");
	close(saved_in);
	if(dup2(saved_out, STDOUT_FILENO) == -1)
		perror("dup failed in builtin edge_case");
	close(saved_out);
	return(1);
}
