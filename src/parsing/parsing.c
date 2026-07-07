/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 16:46:40 by bastalze          #+#    #+#             */
/*   Updated: 2026/06/08 10:53:35 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../minishell.h"

int initiate_parsing(char **env, t_token_node *token_lst, t_token_iteri *iteri)
{
	t_cmd_data				cmd_data;
	t_command				*cmd;

	cmd = ft_calloc(1, sizeof(t_command));
	if(!cmd)
		return (1);
	cmd_data.head = ft_single_lstnew(cmd);
	if(!cmd_data-head)
		return (1);
	cmd_data.tail = cmd_data.head;
	assert(token_lst != NULL);
	assert(cmd_data != NULL);
	if (parsing(env, token_lst, iteri, &cmd_data))
		return (1);
	return(0);
}

int	parsing(char **env, t_token_node *token_lst, t_token_iteri *iteri,
		t_cmd_data *cmd_data)
{
	int	redir;

	bzero(&iteri, sizeof(t_token_iteri));
	while (token_lst[iteri->token].token_type)
	{
		redir = 0;
		if (is_redirection(token_lst, iteri))
		{
			if (redirect(env, token_lst, iteri, cmd_data))
				return (1);
			redir = 1;
		}
		if (is_syntax_error(token_lst, iteri))
		{
			error("syntax error unexpected token");
			return (1);
		} 
		if (!redir)
		{
			if (word_or_pipe(env, token_lst, iteri, cmd_data))
				return (1);
		}
		iteri->token++;
	}
	return (0);
}

int	is_redirection(t_token_node *token_lst, t_token_iteri *iteri)
{
	if (iteri->token != 0 &&
			(token_lst[iteri->token - 1].token_type == REDIR_IN 
			|| token_lst[iteri->token - 1].token_type == REDIR_OUT
			|| token_lst[iteri->token - 1].token_type == REDIR_OUT_A
			|| token_lst[iteri->token - 1].token_type == HERE_DOC)
			&& token_lst[iteri->token].token_type == WORD)
		return (1);
	return (0);
}

int	redirect(char **env, t_token_node *token_lst, t_token_iteri *iteri,
		t_cmd_data *cmd_data)
{
	t_redir_list		*curr_redir;
	t_command		*tmp_cmd;
	t_single_linked_node	*tmp_redir;

	tmp_cmd = (t_command *)cmd_data->tail->content;
	curr_redir = calloc(1, sizeof(t_redir_list));
	if (!curr_redir)
		return (1);
	redir_type_assignment(curr_redir, token_lst);
	curr_redir->filename = calloc(1, ft_strlen(token_lst->token_str) + 1);
	if (!curr_redir->filename)
		return (1);
//	var-expansion_quote-removal;
	ft_strlcpy(curr_redir->filename, token_lst->token_str, ft_strlen(token_lst->token_str) + 1);
	tmp_redir = ft_single_lstnew(curr_redir);
	if (!tmp_redir)
		return (1);
	if (!tmp_cmd->redir)
		tmp_cmd->redir = tmp_redir;
	else
		ft_lstadd_back_single_linked(&tmp_cmd->redir, tmp_redir);
	return (0);
}

void	redir_type_assignment(t_redir_list *curr_redir, t_token_node *token_lst)
{
	if (token_lst->token_type == REDIR_IN)
		curr_redir->redir_type == IN;
	else if (token_lst->token_type == REDIR_OUT)
		curr_redir->redir_type == OUT;
	else if (token_lst->token_type == REDIR_OUT_A)
		curr_redir->redir_type == APPEND;
	else if (token_lst->token_type == HERE_DOC)
		curr_redir->redir_type == HERE;
}

int	syntax_error(t_token_node *token_lst, t_token_iteri *iteri)
{
	if (token_lst[0].token_type == PIPE)
		return (1);
	else if (iteri->token != 0 &&
				(token_lst[iteri->token - 1].token_type == REDIR_IN
				|| token_lst[iteri->token - 1].token_type == REDIR_OUT
				|| token_lst[iteri->token - 1].token_type == REDIR_OUT_A
				|| token_lst[iteri->token - 1].token_type == HERE_DOC
				|| token_lst[iteri->token - 1].token_type == PIPE)
				&& (token_lst[iteri->token].token_type == REDIR_IN
				|| token_lst[iteri->token].token_type == REDIR_OUT
				|| token_lst[iteri->token].token_type == REDIR_OUT_A
				|| token_lst[iteri->token].token_type == HERE_DOC
				|| token_lst[iteri->token].token_type == PIPE))
		return (1);
	else if ((token_lst[iteri->token].token_type == REDIR_IN
				|| token_lst[iteri->token].token_type == REDIR_OUT
				|| token_lst[iteri->token].token_type == REDIR_OUT_A
				|| token_lst[iteri->token].token_type == HERE_DOC
				|| token_lst[iteri->token].token_type == PIPE)
				&& token_lst[iteri->token + 1].token_type == 0)
		return (1);
	return (0);
}

int	word_or_pipe(char **env, t_token_node *token_lst, t_token_iteri *iteri,
		t_cmd_data *cmd_data)
{
	if (token_lst[iteri->token].token_type == WORD)
	{
		//var_expansion;
		//while(array[i])
		//	adding_word_to_argv;
	}
	else if (token_lst[iteri->token].token_type == PIPE)
	{
		if(delimit_command(cmd_data))
			return (1);
	}
}

int	delimit_command(t_cmd_data *cmd_data)
{
	t_command		*new_cmd;
	t_single_linked_node	new_node_ptr;

	new_cmd = calloc(1, sizeof(t_command));
	if(!new_cmd)
		return (1);
	new_node_ptr = ft_single_lstnew(new_cmd);
	if(!tmp_node_ptr)
		return (1);
	ft_lstadd_back_single_linked(&cmd_data->head, new_node_ptr);
	cmd_data->tail = new_node_ptr;
	return (0);
}
