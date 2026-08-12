/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion_dollar_found.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 12:24:32 by bastalze          #+#    #+#             */
/*   Updated: 2026/08/11 17:29:47 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int			dollar_found(char *s, char word[WORD_AMOUNT][WORD_STR_SIZE],
				t_quote_iteri *iteri, t_single_linked_node *env);
static int	is_name(int i, char c);
static int	no_variable(char *s, char word[WORD_AMOUNT][WORD_STR_SIZE],
				t_quote_iteri *iteri, t_single_linked_node *env);
static int	dollar_questionmark(char word[WORD_AMOUNT][WORD_STR_SIZE],
				t_quote_iteri *iteri, t_single_linked_node *env);
static int	dollar_no_var(char word[WORD_AMOUNT][WORD_STR_SIZE],
				t_quote_iteri *iteri);

int	dollar_found(char *s, char word[WORD_AMOUNT][WORD_STR_SIZE],
		t_quote_iteri *iteri, t_single_linked_node *env)
{
	char	var[WORD_STR_SIZE];
	int		v;

	bzero(var, WORD_STR_SIZE - 1);
	assert(s[iteri->i] == '$');
	iteri->i++;
	v = 0;
	while (s[iteri->i] && is_name(v, s[iteri->i]))
	{
		var[v] = s[iteri->i];
		iteri->i++;
		v++;
	}
	if (v != 0)
	{
		var[v] = 0;
		if (find_var(var, word, iteri, env))
			return (1);
	}
	else
	{
		assert(v == 0);
		if (no_variable(s, word, iteri, env))
			return (1);
	}
	return (0);
}
// no safety net for max v needed cause s is maximum WORD_STR_SIZE 
// so var can't be bigger

static int	is_name(int i, char c)
{
	if (i == 0)
	{
		if ((c >= 'a' && c <= 'z')
			|| (c >= 'A' && c <= 'Z')
			|| (c == '_'))
			return (1);
		else
			return (0);
	}
	if (c == '_'
		|| (c >= '0' && c <= '9')
		|| (c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

static int	no_variable(char *s, char word[WORD_AMOUNT][WORD_STR_SIZE],
				t_quote_iteri *iteri, t_single_linked_node *env)
{
	if (s[iteri->i] == '?')
	{
		if (dollar_questionmark(word, iteri, env))
			return (1);
	}
	else
	{
		if (dollar_no_var(word, iteri))
			return (1);
	}
	return (0);
}

static int	dollar_questionmark(char word[WORD_AMOUNT][WORD_STR_SIZE],
				t_quote_iteri *iteri, t_single_linked_node *env)
{
	if(env)
		env = NULL;
	// ATTENTION ENV SET TO NULL!!!!!!!!!!!!!!!!!!!!
	word[iteri->wi][iteri->wj] = '$';
	word[iteri->wi][++iteri->wj] = '?';
	if (iteri->wj + 1 >= WORD_STR_SIZE)
	{
		error("exceeding memory limit: Word length \
			\nRaise WORD_STR_SIZE in minishell.h");
		return (1);
	}
	iteri->wj++;
	iteri->i++;
	return (0);
//		better to directly get the exit status and put it in!
}

static int	dollar_no_var(char word[WORD_AMOUNT][WORD_STR_SIZE],
				t_quote_iteri *iteri)
{
	word[iteri->wi][iteri->wj] = '$';
	if (iteri->wj + 1 >= WORD_STR_SIZE)
	{
		error("exceeding memory limit: Word length \
			\nRaise WORD_STR_SIZE in minishell.h");
		return (1);
	}
	iteri->wj++;
	return (0);
}
