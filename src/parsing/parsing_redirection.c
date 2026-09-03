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

int			is_redirection(t_arena *arena_tokens, t_token_iteri *iteri);
int			redirect(t_single_linked_node *env,
				t_token_iteri *iteri, t_cmd_data *cmd_data,
				t_minishell *mini);
static void	redir_type_assignment(t_redir_list *curr_redir,
				t_token_iteri *iteri);
static int	add_redir_to_struct(t_single_linked_node *env,
				t_token_iteri *iteri, t_redir_list *curr_redir,
				t_minishell *mini);

int	is_redirection(t_arena *arena_tokens, t_token_iteri *iteri)
{
	if (arena_tokens->pos != 0
		&& ((iteri->tok - 1)->token_type == REDIR_IN
			|| (iteri->tok - 1)->token_type == REDIR_OUT
			|| (iteri->tok - 1)->token_type == REDIR_OUT_A
			|| (iteri->tok - 1)->token_type == HERE_DOC)
		&& iteri->tok->token_type == WORD)
		return (1);
	return (0);
}

int	redirect(t_single_linked_node *env,
		t_token_iteri *iteri, t_cmd_data *cmd_data,
		t_minishell *mini)
{
	t_redir_list			*curr_redir;
	t_command				*tmp_cmd;
	t_single_linked_node	*tmp_redir;

	tmp_cmd = (t_command *)cmd_data->tail->content;
	curr_redir = ft_calloc(1, sizeof(t_redir_list));
	if (!curr_redir)
		return (perror("minishell: malloc failed"), mini->exit_status = 1, 1);
	redir_type_assignment(curr_redir, iteri);
	if (add_redir_to_struct(env, iteri, curr_redir, mini))
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
			t_token_iteri *iteri)
{
	if ((iteri->tok - 1)->token_type == REDIR_IN)
		curr_redir->redir_type = IN;
	else if ((iteri->tok - 1)->token_type == REDIR_OUT)
		curr_redir->redir_type = OUT;
	else if ((iteri->tok - 1)->token_type == REDIR_OUT_A)
		curr_redir->redir_type = APPEND;
	else if ((iteri->tok - 1)->token_type == HERE_DOC)
		curr_redir->redir_type = HERE;
}

static int	add_redir_to_struct(t_single_linked_node *env,
		t_token_iteri *iteri, t_redir_list *curr_redir,
		t_minishell *mini)
{
	t_quote_iteri   exv;
	char			**word;

	init_qrve_arena(mini);
	ft_bzero(&exv, sizeof(t_quote_iteri));
	start_first_word(mini, &exv);
	word = (char **)&mini->arena_split_tokens.data;
	if ((iteri->tok - 1)->token_type == HERE_DOC)
	{
		curr_redir->filename = iteri->tok->token_str;
		curr_redir->filename = ft_calloc(1, ft_strlen(iteri->tok->token_str) + 1);
		if (!curr_redir->filename)
			return (perror("minishell: malloc failed"), mini->exit_status = 1, 1);
		ft_strlcpy(curr_redir->filename, iteri->tok->token_str,
			ft_strlen(iteri->tok->token_str) + 1);
		curr_redir->fd = -42;
	}
	else
	{
		if (quote_rm_var_expan(iteri->tok->token_str, mini, env,
			&exv))
			return (1);
		if (word[1] != NULL)
			return (error("ambiguous redirect"), mini->exit_status = 1, 1);
		curr_redir->filename = ft_calloc(1, ft_strlen(word[0]) + 1);
		if (!curr_redir->filename)
		{
			return (perror("minishell: malloc failed"),
				mini->exit_status = 1, 1);
		}
		ft_strlcpy(curr_redir->filename, word[0], ft_strlen(word[0]) + 1);
		curr_redir->fd = -42;
	}
	return (0);
}
