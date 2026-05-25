/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 16:36:21 by bastalze          #+#    #+#             */
/*   Updated: 2026/05/25 15:53:32 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int			tokenization(char *input, char **env, t_token_node *token_lst,
				t_token_iteri *iteri);
static void	here_or_append(char *input, t_token_node *token_lst,
				t_token_iteri *iteri, int i);
static void	operators(char *input, t_token_node *token_lst,
				t_token_iteri *iteri, int i);
static int	quotation_mode(char *input, t_token_node *token_lst,
				t_token_iteri *iteri, int *i);
static void	space_or_word(char *input, t_token_node *token_lst,
				t_token_iteri *iteri, int i);

int	tokenization(char *input, char **env, t_token_node *token_lst,
		t_token_iteri *iteri)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (input[i])
	{
		if (i != 0 && iteri->t == 1 && (input[i] == '>' || input[i] == '<'))
			here_or_append(input, token_lst, iteri, i);
		else if (input[i] == '<' || input[i] == '>' || input[i] == '|')
			operators(input, token_lst, iteri, i);
		else if (input[i] == '\'' || input[i] == '\"')
		{
			token_lst[iteri->token].token_type = WORD;
			status = quotation_mode(input, token_lst, iteri, &i);
			if (status)
				return (1);
		}
		else
			space_or_word(input, token_lst, iteri, i);
		i++;
	}
	tokenization_testing(token_lst, env);
//	grouping(env, token_lst);
	return (0);
}

static void	here_or_append(char *input, t_token_node *token_lst,
		t_token_iteri *iteri, int i)
{
	if (input[i - 1] == '>' && input[i] == '>')
	{
		add_to_token(input[i], token_lst, iteri);
		token_lst[iteri->token].token_type = REDIR_OUT_A;
		delimit_token(iteri);
	}
	else if (input[i - 1] == '<' && input[i] == '<')
	{
		add_to_token(input[i], token_lst, iteri);
		token_lst[iteri->token].token_type = HERE_DOC;
		delimit_token(iteri);
//			here_doc(input, token_lst, iteri, &i);
	}
}

static void	operators(char *input, t_token_node *token_lst,
		t_token_iteri *iteri, int i)
{
	if (input[i] == '<')
	{
		add_to_token(input[i], token_lst, iteri);
		token_lst[iteri->token].token_type = REDIR_IN;
		if (input[i + 1] != '<')
			delimit_token(iteri);
	}
	else if (input[i] == '>')
	{
		add_to_token(input[i], token_lst, iteri);
		token_lst[iteri->token].token_type = REDIR_OUT;
		if (input[i + 1] != '>')
			delimit_token(iteri);
	}
	else if (input[i] == '|')
	{
		add_to_token(input[i], token_lst, iteri);
		token_lst[iteri->token].token_type = PIPE;
		delimit_token(iteri);
	}
}

static int	quotation_mode(char *input, t_token_node *token_lst,
		t_token_iteri *iteri, int *i)
{
	char	c;

	c = input[*i];
	add_to_token(input[*i], token_lst, iteri);
	(*i)++;
	while (input[*i] != c && input[*i] != 0)
	{
		add_to_token(input[*i], token_lst, iteri);
		(*i)++;
	}
	if (input[*i] == c)
	{
		add_to_token(input[*i], token_lst, iteri);
		if (input[*i + 1] == '>' || input[*i + 1] == '<'
			|| input[*i + 1] == '|')
			delimit_token(iteri);
		return (0);
	}
	else
	{
		write(2, "Quotation not closed\n", 21);
		return (1);
	}
}

static void	space_or_word(char *input, t_token_node *token_lst,
		t_token_iteri *iteri, int i)
{
	if (input[i] == ' ' || input[i] == '\t' || input[i] == '\v')
	{
		if (token_lst[iteri->token].token_str[0] != 0)
			delimit_token(iteri);
	}
	else if (iteri->t != 0)
	{
		add_to_token(input[i], token_lst, iteri);
		if (input[i + 1] == '>' || input[i + 1] == '<' || input[i + 1] == '|')
			delimit_token(iteri);
	}
	else
	{
		add_to_token(input[i], token_lst, iteri);
		token_lst[iteri->token].token_type = WORD;
	}
}
