/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 16:36:21 by bastalze          #+#    #+#             */
/*   Updated: 2026/05/29 22:11:23 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../minishell.h"

int			tokenization(char *input, char **env, t_token_node *token_lst,
				t_token_iteri *iteri);
static void	here_or_append(char *input, char **env, t_token_node *token_lst,
				t_token_iteri *iteri);
static void	operators(char *input, char **env, t_token_node *token_lst,
				t_token_iteri *iteri);
static int	quotation_mode(char *input, char **env, t_token_node *token_lst,
				t_token_iteri *iteri);
static void	space_or_word(char *input, char **env, t_token_node *token_lst,
				t_token_iteri *iteri);

int	tokenization(char *input, char **env, t_token_node *token_lst,
		t_token_iteri *iteri)
{
	int	status;

	status = 0;
	while (input[iteri->i])
	{
		if (iteri->i != 0 && iteri->t == 1 && (input[iteri->i] == '>' || input[iteri->i] == '<'))
			here_or_append(input, env, token_lst, iteri);
		else if (input[iteri->i] == '<' || input[iteri->i] == '>' || input[iteri->i] == '|')
			operators(input, env, token_lst, iteri);
		else if (input[iteri->i] == '\'' || input[iteri->i] == '\"')
		{
			token_lst[iteri->token].token_type = WORD;
			status = quotation_mode(input, env, token_lst, iteri);
			if (status)
				return (1);
		}
		else
			space_or_word(input, env, token_lst, iteri);
		iteri->i++;
	}
	delimit_token(input, env, token_lst, iteri);
	tokenization_testing(token_lst, env);
//	grouping(env, token_lst);
	return (0);
}

static void	here_or_append(char *input, char **env, t_token_node *token_lst,
		t_token_iteri *iteri)
{
	if (input[iteri->i - 1] == '>' && input[iteri->i] == '>')
	{
		add_to_token(input[iteri->i], token_lst, iteri);
		token_lst[iteri->token].token_type = REDIR_OUT_A;
		delimit_token(input, env, token_lst, iteri);
	}
	else if (input[iteri->i - 1] == '<' && input[iteri->i] == '<')
	{
		add_to_token(input[iteri->i], token_lst, iteri);
		token_lst[iteri->token].token_type = HERE_DOC;
	}
}

static void	operators(char *input, char **env, t_token_node *token_lst,
		t_token_iteri *iteri)
{
	if (input[iteri->i] == '<')
	{
		add_to_token(input[iteri->i], token_lst, iteri);
		token_lst[iteri->token].token_type = REDIR_IN;
		if (input[iteri->i + 1] != '<')
			delimit_token(input, env, token_lst, iteri);
	}
	else if (input[iteri->i] == '>')
	{
		add_to_token(input[iteri->i], token_lst, iteri);
		token_lst[iteri->token].token_type = REDIR_OUT;
		if (input[iteri->i + 1] != '>')
			delimit_token(input, env, token_lst, iteri);
	}
	else if (input[iteri->i] == '|')
	{
		add_to_token(input[iteri->i], token_lst, iteri);
		token_lst[iteri->token].token_type = PIPE;
		delimit_token(input, env, token_lst, iteri);
	}
}

static int	quotation_mode(char *input, char **env, t_token_node *token_lst,
		t_token_iteri *iteri)
{
	char	c;

	c = input[iteri->i];
	add_to_token(input[iteri->i], token_lst, iteri);
	iteri->i++;
	while (input[iteri->i] != c && input[iteri->i] != 0)
	{
		add_to_token(input[iteri->i], token_lst, iteri);
		iteri->i++;
	}
	if (input[iteri->i])
	{
		add_to_token(input[iteri->i], token_lst, iteri);
		if (input[iteri->i + 1] == '>' || input[iteri->i + 1] == '<'
			|| input[iteri->i + 1] == '|')
			delimit_token(input, env, token_lst, iteri);
		return (0);
	}
	else
	{
		write(2, "Quotation not closed\n", 21);
		return (1);
	}
}

static void	space_or_word(char *input, char **env, t_token_node *token_lst,
		t_token_iteri *iteri)
{
	if (input[iteri->i] == ' ' || input[iteri->i] == '\t' || input[iteri->i] == '\v')
	{
		if (token_lst[iteri->token].token_str[0] != 0)
			delimit_token(input, env, token_lst, iteri);
	}
	else if (iteri->t != 0)
	{
		add_to_token(input[iteri->i], token_lst, iteri);
		if (input[iteri->i + 1] == '>' || input[iteri->i + 1] == '<' || input[iteri->i + 1] == '|')
			delimit_token(input, env, token_lst, iteri);
	}
	else
	{
		add_to_token(input[iteri->i], token_lst, iteri);
		token_lst[iteri->token].token_type = WORD;
	}
}
