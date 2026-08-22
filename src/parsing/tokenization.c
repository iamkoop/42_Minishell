/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 16:36:21 by bastalze          #+#    #+#             */
/*   Updated: 2026/08/20 21:07:04 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int		tokenization(char *input, t_single_linked_node **env,
				t_minishell *mini, t_token_iteri *iteri);
static int	quotation_mode(char *input, t_single_linked_node *env,
				t_minishell *mini, t_token_iteri *iteri);
static int	space_or_word(char *input, t_single_linked_node *env,
				t_minishell *mini, t_token_iteri *iteri);
static int	word(char *input, t_single_linked_node *env,
				t_minishell *mini, t_token_iteri *iteri);

int	tokenization(char *input, t_single_linked_node **env,
		t_minishell *mini, t_token_iteri *iteri)
{
	while (input[iteri->i])
	{
		if (input[iteri->i] == '<' || input[iteri->i] == '>'
			|| input[iteri->i] == '|')
		{
			if (operators1(input, *env, mini, iteri))
				return (1);
		}
		else if (input[iteri->i] == '\'' || input[iteri->i] == '\"')
		{
			mini->token_lst[iteri->token].token_type = WORD;
			if (quotation_mode(input, *env, mini, iteri))
				return (1);
		}
		else
		{
			if (space_or_word(input, *env, mini, iteri))
				return (1);
		}
		iteri->i++;
	}
	delimit_token(input, *env, mini, iteri);
//	tokenization_testing(token_lst, env);
	if(initiate_parsing(env, mini, iteri))
		return (1);
	return (0);
}

static int	quotation_mode(char *input, t_single_linked_node *env,
		t_minishell *mini, t_token_iteri *iteri)
{
	char	c;

	c = input[iteri->i];
	add_to_token(input[iteri->i], mini->token_lst, iteri);
	iteri->i++;
	while (input[iteri->i] != c && input[iteri->i] != 0)
	{
		if (add_to_token(input[iteri->i], mini->token_lst, iteri))
			return (1);
		iteri->i++;
	}
	if (input[iteri->i] == c)
	{
		if (add_to_token(input[iteri->i], mini->token_lst, iteri))
			return (1);
		if (input[iteri->i + 1] == '>' || input[iteri->i + 1] == '<'
			|| input[iteri->i + 1] == '|')
			delimit_token(input, env, mini, iteri);
		return (0);
	}
	else
		return (error("Quotation not closed"), 1);
}

static int	space_or_word(char *input, t_single_linked_node *env,
		t_minishell *mini, t_token_iteri *iteri)
{
	if (input[iteri->i] == ' ' || input[iteri->i] == '\t'
		|| input[iteri->i] == '\v')
	{
		if (mini->token_lst[iteri->token].token_str[0] != 0)
		{
			if (delimit_token(input, env, mini, iteri))
				return (1);
		}
	}
	else
	{
		if (word(input, env, mini, iteri))
			return (1);
	}
	return (0);
}

int	word(char *input, t_single_linked_node *env,
		t_minishell *mini, t_token_iteri *iteri)
{
	if (iteri->t != 0)
	{
		if (add_to_token(input[iteri->i], mini->token_lst, iteri))
			return (1);
		if (input[iteri->i + 1] == '>' || input[iteri->i + 1] == '<'
			|| input[iteri->i + 1] == '|')
		{
			if (delimit_token(input, env, mini, iteri))
				return (1);
		}
	}
	else
	{
		if (add_to_token(input[iteri->i], mini->token_lst, iteri))
			return (1);
		mini->token_lst[iteri->token].token_type = WORD;
	}
	return (0);
}
