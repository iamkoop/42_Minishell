/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal_var_expansion.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 16:27:55 by bastalze          #+#    #+#             */
/*   Updated: 2026/08/20 21:34:45 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	quote_rm_var_expan(char *s, char word[WORD_AMOUNT][WORD_STR_SIZE],
		t_single_linked_node *env, t_quote_iteri *iteri);
int	quote_mode(char *s, char word[WORD_AMOUNT][WORD_STR_SIZE],
		t_quote_iteri *iteri, t_single_linked_node *env);
int	var_ex_add_char(char word[WORD_AMOUNT][WORD_STR_SIZE], char *s,
		t_single_linked_node *env, t_quote_iteri *iteri);
int	add_char(char word[WORD_AMOUNT][WORD_STR_SIZE], char *s,
		t_quote_iteri *iteri);

int	quote_rm_var_expan(char *s, char word[WORD_AMOUNT][WORD_STR_SIZE],
		t_single_linked_node *env, t_quote_iteri *iteri)
{
	while (s[iteri->i])
	{
		if ((s[iteri->i] == '\'' || s[iteri->i] == '\"') && iteri->heredoc == false)
		{
			if (quote_mode(s, word, iteri, env))
				return (1);
		}
		else
		{
			if (var_ex_add_char(word, s, env, iteri))
				return (1);
		}
	}
	word[iteri->wi][iteri->wj] = 0;
	return (0);
}

int	quote_mode(char *s, char word[WORD_AMOUNT][WORD_STR_SIZE],
		t_quote_iteri *iteri, t_single_linked_node *env)
{
	char	c;

	c = s[iteri->i];
	iteri->quoted = true;
	assert(s[iteri->i] == '\'' || s[iteri->i] == '\"');
	iteri->i++;
	while (s[iteri->i] && s[iteri->i] != c)
	{
		if (s[iteri->i] == '$' && c == '\"')
		{
			if (dollar_found(s, word, iteri, env))
				return (1);
		}
		else
		{
			if (add_char(word, s, iteri))
				return (1);
		}
	}
	assert(s[iteri->i] == '\'' || s[iteri->i] == '\"');
	iteri->i++;
	iteri->quoted = false;
	return (0);
}

int	var_ex_add_char(char word[WORD_AMOUNT][WORD_STR_SIZE], char *s,
	t_single_linked_node *env, t_quote_iteri *iteri)
{
	if (s[iteri->i] == '$')
	{
		if (dollar_found(s, word, iteri, env))
			return (1);
	}
	else
	{
		if (add_char(word, s, iteri))
			return (1);
	}
	return (0);
}

int	add_char(char word[WORD_AMOUNT][WORD_STR_SIZE], char *s,
	t_quote_iteri *iteri)
{
	word[iteri->wi][iteri->wj] = s[iteri->i];
	if (iteri->wj + 1 >= WORD_STR_SIZE)
	{
		error("exceeding memory limit: Word length \
				\nRaise WORD_STR_SIZE in minishell.h");
		return (1);
	}
	iteri->i++;
	iteri->wj++;
	return (0);
}
