/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 16:46:40 by bastalze          #+#    #+#             */
/*   Updated: 2026/08/21 18:53:40 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int			initiate_parsing(t_single_linked_node **env,
				t_minishell *mini, t_token_iteri *iteri);
int			parsing(t_single_linked_node **env, t_minishell *mini,
				t_token_iteri *iteri, t_cmd_data *cmd_data);
static int	is_syntax_error(t_arena *arena_tokens, t_token_iteri *iteri);
static int	word_or_pipe(t_single_linked_node *env, t_minishell *mini,
			t_token_iteri *iteri, t_cmd_data *cmd_data);
static int	delimit_command(t_cmd_data *cmd_data);

int	initiate_parsing(t_single_linked_node **env,
		t_minishell *mini, t_token_iteri *iteri)
{
	t_cmd_data				cmd_data;
	t_command				*cmd;

	cmd = ft_calloc(1, sizeof(t_command));
	if (!cmd)
		return (1);
	cmd_data.head = ft_single_lstnew(cmd);
	if (!cmd_data.head)
		return (free(cmd), 1);
	cmd_data.tail = cmd_data.head;
	assert(cmd_data.head != NULL);
	ft_bzero(iteri, sizeof(t_token_iteri));
	iteri->tok = (t_token_node *)mini->arena_tokens.data;
	if (parsing(env, mini, iteri, &cmd_data))
		return (free_command_struct(cmd_data.head), 1);
	return (free_command_struct(cmd_data.head), 0);
}

int	parsing(t_single_linked_node **env, t_minishell *mini,
		t_token_iteri *iteri, t_cmd_data *cmd_data)
{
	int	is_redir;

	while (iteri->tok->token_type)
	{
		is_redir = 0;
		if (is_redirection(&mini->arena_tokens, iteri))
		{
			if (redirect(*env, iteri, cmd_data, mini))
				return (1);
			is_redir = 1;
		}
		if (is_syntax_error(&mini->arena_tokens, iteri))
			return (error("syntax error unexpected token"), 1);
		if (!is_redir)
		{
			if (word_or_pipe(*env, mini, iteri, cmd_data))
				return (1);
		}
		iteri->tok++;
	}
//	printing_struct_content(cmd_data);
	exec_main(mini, cmd_data->head, env);
	return (0);
}

static int	is_syntax_error(t_arena *arena_tokens, t_token_iteri *iteri)
{
	if (((t_token_node *)arena_tokens->data)->token_type == PIPE)
		return (1);
	else if (arena_tokens->pos != 0
		&& ((iteri->tok - 1)->token_type == REDIR_IN
			|| (iteri->tok - 1)->token_type == REDIR_OUT
			|| (iteri->tok - 1)->token_type == REDIR_OUT_A
			|| (iteri->tok - 1)->token_type == HERE_DOC)
		&& (iteri->tok->token_type == REDIR_IN
			|| iteri->tok->token_type == REDIR_OUT
			|| iteri->tok->token_type == REDIR_OUT_A
			|| iteri->tok->token_type == HERE_DOC
			|| iteri->tok->token_type == PIPE))
		return (1);
	else if (iteri->tok->token_type == PIPE
		&& (iteri->tok + 1)->token_type == PIPE)
		return (1);
	else if ((iteri->tok->token_type == REDIR_IN
			|| iteri->tok->token_type == REDIR_OUT
			|| iteri->tok->token_type == REDIR_OUT_A
			|| iteri->tok->token_type == HERE_DOC
			|| iteri->tok->token_type == PIPE)
		&& (iteri->tok + 1)->token_type == 0)
		return (1);
	return (0);
}

static int	word_or_pipe(t_single_linked_node *env, t_minishell *mini,
			t_token_iteri *iteri, t_cmd_data *cmd_data)
{
	t_quote_iteri	exv;

	init_qrve_arena(mini);
	ft_bzero(&exv, sizeof(t_quote_iteri));
	start_first_word(mini, &exv);
	exv.exit_status = mini->exit_status;
	if (iteri->tok->token_type == WORD)
	{
//		printf("word before quote removal: %s\n", iteri->tok->token_str);
		if (quote_rm_var_expan(iteri->tok->token_str, mini, env,
				&exv))
			return (1);
		if (add_word_to_struct(cmd_data, mini))
		{
			perror("minishell: Malloc for argv in parsing failed");
			return (1);
		}
	}
	else if (iteri->tok->token_type == PIPE)
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

	new_cmd = ft_calloc(1, sizeof(t_command));
	if (!new_cmd)
		return (1);
	new_node_ptr = ft_single_lstnew(new_cmd);
	if (!new_node_ptr)
		return (1);
	ft_lstadd_back_single_linked(&cmd_data->head, new_node_ptr);
	cmd_data->tail = new_node_ptr;
	return (0);
}
