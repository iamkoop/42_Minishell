/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion_dollar_found.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 12:24:32 by bastalze          #+#    #+#             */
/*   Updated: 2026/08/27 16:12:59 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int			dollar_found(char *s, t_minishell *mini,
				t_quote_iteri *iteri, t_single_linked_node *env);
static int	is_name(int i, char c);
static int	no_variable(char *s, t_minishell *mini,
				t_quote_iteri *iteri);
static int	dollar_questionmark(t_minishell *mini,
				t_quote_iteri *iteri);
static int	dollar_no_var(t_minishell *mini,
				t_quote_iteri *iteri);

int	dollar_found(char *s, t_minishell *mini,
		t_quote_iteri *iteri, t_single_linked_node *env)
{
	char	var[WORD_STR_SIZE];
	int		v;

	ft_bzero(var, WORD_STR_SIZE - 1);
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
		if (find_var(var, mini, iteri, env))
			return (1);
	}
	else
	{
		assert(v == 0);
		if (no_variable(s, mini, iteri))
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

static int	no_variable(char *s, t_minishell *mini,
				t_quote_iteri *iteri)
{
	if (s[iteri->i] == '?')
	{
		if (dollar_questionmark(mini, iteri))
			return (1);
	}
	else
	{
		if (dollar_no_var(mini, iteri))
			return (1);
	}
	return (0);
}

static int	dollar_questionmark(t_minishell *mini,
				t_quote_iteri *iteri)
{
	char	*exit_status_word;
	int	i;

	exit_status_word = ft_itoa(iteri->exit_status);
	if (!exit_status_word)
		return (perror("minishell: malloc failed"), mini->exit_status = 1, 1);
	i = 0;
	while (exit_status_word[i])
	{
		if (add_to_word(exit_status_word[i], mini, iteri))
			return (1);
		i++;
	}
	free(exit_status_word);
	iteri->i++;
	return (0);
}

static int	dollar_no_var(t_minishell *mini,
				t_quote_iteri *iteri)
{
	if (add_to_word('$', mini, iteri))
		return (1);
	return (0);
}
