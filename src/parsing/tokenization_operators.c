/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_operators.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 19:21:28 by bastalze          #+#    #+#             */
/*   Updated: 2026/08/20 21:33:51 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	operators1(char *input, t_single_linked_node *env,
		t_token_node *token_lst, t_token_iteri *iteri);
int	operators2(char *input, t_single_linked_node *env,
		t_token_node *token_lst, t_token_iteri *iteri);
int	here_or_append(char *input, t_single_linked_node *env,
		t_token_node *token_lst, t_token_iteri *iteri);
int	redirections(char *input, t_single_linked_node *env,
		t_token_node *token_lst, t_token_iteri *iteri);

int	operators1(char *input, t_single_linked_node *env,
		t_token_node *token_lst, t_token_iteri *iteri)
{
	if (iteri->i != 0 && ((input[iteri->i - 1] == '>'
				&& input[iteri->i] == '>')
			|| (input[iteri->i - 1] == '<' && input[iteri->i] == '<')))
	{
		if (here_or_append(input, env, token_lst, iteri))
			return (1);
	}
	else if (input[iteri->i] == '<' || input[iteri->i] == '>'
		|| input[iteri->i] == '|')
	{
		if (operators2(input, env, token_lst, iteri))
			return (1);
	}
	return (0);
}

int	operators2(char *input, t_single_linked_node *env,
		t_token_node *token_lst, t_token_iteri *iteri)
{
	if (add_to_token(input[iteri->i], token_lst, iteri))
		return (1);
	if (input[iteri->i] == '<' || input[iteri->i] == '>')
	{
		if (redirections(input, env, token_lst, iteri))
			return (1);
	}
	else if (input[iteri->i] == '|')
	{
		token_lst[iteri->token].token_type = PIPE;
		if (delimit_token(input, env, token_lst, iteri))
			return (1);
	}
	return (0);
}

int	here_or_append(char *input, t_single_linked_node *env,
		t_token_node *token_lst, t_token_iteri *iteri)
{
	if (input[iteri->i - 1] == '>' && input[iteri->i] == '>')
	{
		if (add_to_token(input[iteri->i], token_lst, iteri))
			return (1);
		token_lst[iteri->token].token_type = REDIR_OUT_A;
		if (delimit_token(input, env, token_lst, iteri))
			return (1);
	}
	else if (input[iteri->i - 1] == '<' && input[iteri->i] == '<')
	{
		if (add_to_token(input[iteri->i], token_lst, iteri))
			return (1);
		token_lst[iteri->token].token_type = HERE_DOC;
		if (delimit_token(input, env, token_lst, iteri))
			return (1);
	}
	return (0);
}

int	redirections(char *input, t_single_linked_node *env,
		t_token_node *token_lst, t_token_iteri *iteri)
{
	if (input[iteri->i] == '<')
	{
		token_lst[iteri->token].token_type = REDIR_IN;
		if (input[iteri->i + 1] != '<')
		{
			if (delimit_token(input, env, token_lst, iteri))
				return (1);
		}
	}
	else if (input[iteri->i] == '>')
	{
		token_lst[iteri->token].token_type = REDIR_OUT;
		if (input[iteri->i + 1] != '>')
		{
			if (delimit_token(input, env, token_lst, iteri))
				return (1);
		}
	}
	return (0);
}
