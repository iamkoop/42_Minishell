/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 10:55:45 by bastalze          #+#    #+#             */
/*   Updated: 2026/08/12 15:17:56 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int			is_redirection(t_token_node *token_lst, t_token_iteri *iteri);
int			redirect(t_single_linked_node *env, t_token_node *token_lst,
				t_token_iteri *iteri, t_cmd_data *cmd_data);
static void	redir_type_assignment(t_redir_list *curr_redir,
				t_token_node *token_lst, t_token_iteri *iteri);
static int	add_redir_to_struct(t_single_linked_node *env,
				t_token_node *token_lst, t_token_iteri *iteri,
				t_redir_list *curr_redir);

int	is_redirection(t_token_node *token_lst, t_token_iteri *iteri)
{
	if (iteri->token != 0
		&& (token_lst[iteri->token - 1].token_type == REDIR_IN
			|| token_lst[iteri->token - 1].token_type == REDIR_OUT
			|| token_lst[iteri->token - 1].token_type == REDIR_OUT_A
			|| token_lst[iteri->token - 1].token_type == HERE_DOC)
		&& token_lst[iteri->token].token_type == WORD)
		return (1);
	return (0);
}

int	redirect(t_single_linked_node *env, t_token_node *token_lst,
		t_token_iteri *iteri, t_cmd_data *cmd_data)
{
	t_redir_list			*curr_redir;
	t_command				*tmp_cmd;
	t_single_linked_node	*tmp_redir;

	tmp_cmd = (t_command *)cmd_data->tail->content;
	curr_redir = ft_calloc(1, sizeof(t_redir_list));
	if (!curr_redir)
		return (1);
	redir_type_assignment(curr_redir, token_lst, iteri);
	if (add_redir_to_struct(env, token_lst, iteri, curr_redir))
		return (1);
	tmp_redir = ft_single_lstnew(curr_redir);
	if (!tmp_redir)
		return (1);
	if (!tmp_cmd->redir)
		tmp_cmd->redir = tmp_redir;
	else
		ft_lstadd_back_single_linked(&tmp_cmd->redir, tmp_redir);
	return (0);
}

static void	redir_type_assignment(t_redir_list *curr_redir,
			t_token_node *token_lst, t_token_iteri *iteri)
{
	if (token_lst[iteri->token - 1].token_type == REDIR_IN)
		curr_redir->redir_type = IN;
	else if (token_lst[iteri->token - 1].token_type == REDIR_OUT)
		curr_redir->redir_type = OUT;
	else if (token_lst[iteri->token - 1].token_type == REDIR_OUT_A)
		curr_redir->redir_type = APPEND;
	else if (token_lst[iteri->token - 1].token_type == HERE_DOC)
		curr_redir->redir_type = HERE;
}

static int	add_redir_to_struct(t_single_linked_node *env,
		t_token_node *token_lst, t_token_iteri *iteri, t_redir_list *curr_redir)
{
	char	word[WORD_AMOUNT][WORD_STR_SIZE];
	t_quote_iteri   exv;

        ft_bzero(&exv, sizeof(t_quote_iteri));
	ft_bzero(word, WORD_AMOUNT * WORD_STR_SIZE);
	if (token_lst[iteri->token - 1].token_type == HERE_DOC)
		curr_redir->fd = ft_atoi(token_lst[iteri->token].token_str);
	else
	{
		if (quote_rm_var_expan(token_lst[iteri->token].token_str, word, env,
			&exv))
			return (1);
		if (word[1][0] != 0)
			return (error("ambiguous redirect"), 1);
		curr_redir->filename = calloc(1, ft_strlen(word[0]) + 1);
		if (!curr_redir->filename)
			return (1);
		ft_strlcpy(curr_redir->filename, word[0], ft_strlen(word[0]) + 1);
		curr_redir->fd = -42;
	}
	return (0);
}
