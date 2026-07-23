/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 16:36:21 by bastalze          #+#    #+#             */
/*   Updated: 2026/07/23 18:39:54 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../minishell.h"

int			tokenization(char *input, t_single_linked_node *env,
				t_token_node *token_lst, t_token_iteri *iteri);
static int	here_or_append(char *input, t_single_linked_node *env,
				t_token_node *token_lst, t_token_iteri *iteri);
static int	operators(char *input, t_single_linked_node *env,
				t_token_node *token_lst, t_token_iteri *iteri);
static int	quotation_mode(char *input, t_single_linked_node *env,
				t_token_node *token_lst, t_token_iteri *iteri);
static int	space_or_word(char *input, t_single_linked_node *env,
				t_token_node *token_lst, t_token_iteri *iteri);

int	tokenization(char *input, t_single_linked_node *env,
		t_token_node *token_lst, t_token_iteri *iteri)
{
	while (input[iteri->i])
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
			if (operators(input, env, token_lst, iteri))
				return (1);
		}
		else if (input[iteri->i] == '\'' || input[iteri->i] == '\"')
		{
			token_lst[iteri->token].token_type = WORD;
			if (quotation_mode(input, env, token_lst, iteri))
				return (1);
		}
		else
		{
			if (space_or_word(input, env, token_lst, iteri))
				return (1);
		}
		iteri->i++;
	}
	delimit_token(input, env, token_lst, iteri);
//	tokenization_testing(token_lst, env);
//	if(!initiate_parsing(env, token_lst, iteri))
//		return (1);
	return (0);
}

static int	here_or_append(char *input, t_single_linked_node *env,
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

static int	operators(char *input, t_single_linked_node *env,
		t_token_node *token_lst, t_token_iteri *iteri)
{
	if (add_to_token(input[iteri->i], token_lst, iteri))
		return (1);
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
	else if (input[iteri->i] == '|')
	{
		token_lst[iteri->token].token_type = PIPE;
		if (delimit_token(input, env, token_lst, iteri))
			return (1);
	}
	return (0);
}

static int	quotation_mode(char *input, t_single_linked_node *env,
		t_token_node *token_lst, t_token_iteri *iteri)
{
	char	c;

	c = input[iteri->i];
	add_to_token(input[iteri->i], token_lst, iteri);
	iteri->i++;
	while (input[iteri->i] != c && input[iteri->i] != 0)
	{
		if (add_to_token(input[iteri->i], token_lst, iteri))
			return (1);
		iteri->i++;
	}
	if (input[iteri->i] == c)
	{
		if (add_to_token(input[iteri->i], token_lst, iteri))
			return (1);
		if (input[iteri->i + 1] == '>' || input[iteri->i + 1] == '<'
			|| input[iteri->i + 1] == '|')
			delimit_token(input, env, token_lst, iteri);
		return (0);
	}
	else
	{
		error("Quotation not closed");
		return (1);
	}
	return (0);
}

static int	space_or_word(char *input, t_single_linked_node *env,
		t_token_node *token_lst, t_token_iteri *iteri)
{
	if (input[iteri->i] == ' ' || input[iteri->i] == '\t'
		|| input[iteri->i] == '\v')
	{
		if (token_lst[iteri->token].token_str[0] != 0)
		{
			if (delimit_token(input, env, token_lst, iteri))
				return (1);
		}
	}
	else if (iteri->t != 0)
	{
		if (add_to_token(input[iteri->i], token_lst, iteri))
			return (1);
		if (input[iteri->i + 1] == '>' || input[iteri->i + 1] == '<'
			|| input[iteri->i + 1] == '|')
		{
			if (delimit_token(input, env, token_lst, iteri))
				return (1);
		}
	}
	else
	{
		if (add_to_token(input[iteri->i], token_lst, iteri))
			return (1);
		token_lst[iteri->token].token_type = WORD;
	}
	return (0);
}
