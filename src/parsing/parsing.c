/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 16:46:40 by bastalze          #+#    #+#             */
/*   Updated: 2026/08/19 17:26:41 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int			initiate_parsing(t_single_linked_node *env,
				t_minishell *mini, t_token_iteri *iteri);
int			parsing(t_single_linked_node *env, t_minishell *mini,
				t_token_iteri *iteri, t_cmd_data *cmd_data);
static int	is_syntax_error(t_token_node *token_lst, t_token_iteri *iteri);
static int	word_or_pipe(t_single_linked_node *env, t_minishell *mini,
			t_token_iteri *iteri, t_cmd_data *cmd_data);
static int	delimit_command(t_cmd_data *cmd_data);

int	initiate_parsing(t_single_linked_node *env,
		t_minishell *mini, t_token_iteri *iteri)
{
	t_cmd_data				cmd_data;
	t_command				*cmd;

	cmd = ft_calloc(1, sizeof(t_command));
	if (!cmd)
		return (1);
	cmd_data.head = ft_single_lstnew(cmd);
	if (!cmd_data.head)
		return (1);
	cmd_data.tail = cmd_data.head;
	assert(mini->token_lst != NULL);
	assert(cmd_data.head != NULL);
	if (parsing(env, mini, iteri, &cmd_data))
		return (free_command_struct(&cmd_data), 1);
	return (free_command_struct(&cmd_data), 0);
}

int	parsing(t_single_linked_node *env, t_minishell *mini,
		t_token_iteri *iteri, t_cmd_data *cmd_data)
{
	int	is_redir;

	ft_bzero(iteri, sizeof(t_token_iteri));
	while (mini->token_lst[iteri->token].token_type)
	{
		is_redir = 0;
		if (is_redirection(mini->token_lst, iteri))
		{
			if (redirect(env, mini->token_lst, iteri, cmd_data))
				return (1);
			is_redir = 1;
		}
		if (is_syntax_error(mini->token_lst, iteri))
			return (error("syntax error unexpected token"), 1);
		if (!is_redir)
		{
			if (word_or_pipe(env, mini, iteri, cmd_data))
				return (1);
		}
		iteri->token++;
	}
//	printing_struct_content(cmd_data);
	exec_main(mini, cmd_data->head, env);
	return (0);
}

static int	is_syntax_error(t_token_node *token_lst, t_token_iteri *iteri)
{
	if (token_lst[0].token_type == PIPE)
		return (1);
	else if (iteri->token != 0
		&& (token_lst[iteri->token - 1].token_type == REDIR_IN
			|| token_lst[iteri->token - 1].token_type == REDIR_OUT
			|| token_lst[iteri->token - 1].token_type == REDIR_OUT_A
			|| token_lst[iteri->token - 1].token_type == HERE_DOC)
		&& (token_lst[iteri->token].token_type == REDIR_IN
			|| token_lst[iteri->token].token_type == REDIR_OUT
			|| token_lst[iteri->token].token_type == REDIR_OUT_A
			|| token_lst[iteri->token].token_type == HERE_DOC
			|| token_lst[iteri->token].token_type == PIPE))
		return (1);
	else if (token_lst[iteri->token].token_type == PIPE
		&& token_lst[iteri->token + 1].token_type == PIPE)
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

static int	word_or_pipe(t_single_linked_node *env, t_minishell *mini,
			t_token_iteri *iteri, t_cmd_data *cmd_data)
{
	char		word[WORD_AMOUNT][WORD_STR_SIZE];
	t_quote_iteri	exv;

	ft_bzero(word, (WORD_AMOUNT * WORD_STR_SIZE));
	ft_bzero(&exv, sizeof(t_quote_iteri));
	exv.exit_status = mini->exit_status;
	if (mini->token_lst[iteri->token].token_type == WORD)
	{
		if (quote_rm_var_expan(mini->token_lst[iteri->token].token_str, word, env,
				&exv))
			return (1);
		if (add_word_to_struct(cmd_data, word))
		{
			perror("minishell: Malloc for argv in parsing failed");
			return (1);
		}
	}
	else if (mini->token_lst[iteri->token].token_type == PIPE)
	{
		if (delimit_command(cmd_data))
			return (1);
	}
	return (0);
}

static int	delimit_command(t_cmd_data *cmd_data)
{
	t_command				*new_cmd;
	t_single_linked_node	*new_node_ptr;

	new_cmd = calloc(1, sizeof(t_command));
	if (!new_cmd)
		return (1);
	new_node_ptr = ft_single_lstnew(new_cmd);
	if (!new_node_ptr)
		return (1);
	ft_lstadd_back_single_linked(&cmd_data->head, new_node_ptr);
	cmd_data->tail = new_node_ptr;
	return (0);
}
