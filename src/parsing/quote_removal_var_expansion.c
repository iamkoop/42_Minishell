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

int	quote_rm_var_expan(char *s, t_minishell *mini,
		t_single_linked_node *env, t_quote_iteri *iteri);
int	quote_mode(char *s, t_minishell *mini,
		t_quote_iteri *iteri, t_single_linked_node *env);
int	var_ex_add_char(t_minishell *mini, char *s,
		t_single_linked_node *env, t_quote_iteri *iteri);
int	add_char(t_minishell *mini, char *s,
		t_quote_iteri *iteri);

int	quote_rm_var_expan(char *s, t_minishell *mini,
		t_single_linked_node *env, t_quote_iteri *iteri)
{
//	init_qrve(mini);
	
	while (s[iteri->i])
	{
		if ((s[iteri->i] == '\'' || s[iteri->i] == '\"') && iteri->heredoc == false)
		{
			if (quote_mode(s, mini, iteri, env))
				return (1);
		}
		else
		{
			if (var_ex_add_char(mini, s, env, iteri))
				return (1);
		}
	}
//	word[iteri->wi][iteri->wj] = 0;
	return (0);
}

int	quote_mode(char *s, t_minishell *mini,
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
			if (dollar_found(s, mini, iteri, env))
				return (1);
		}
		else
		{
			if (add_char(mini, s, iteri))
				return (1);
		}
	}
	assert(s[iteri->i] == '\'' || s[iteri->i] == '\"');
	iteri->i++;
	iteri->quoted = false;
	return (0);
}

int	var_ex_add_char(t_minishell *mini, char *s,
	t_single_linked_node *env, t_quote_iteri *iteri)
{
	if (s[iteri->i] == '$')
	{
		if (dollar_found(s, mini, iteri, env))
			return (1);
	}
	else
	{
		if (add_char(mini, s, iteri))
			return (1);
	}
	return (0);
}

int	add_char(t_minishell *mini, char *s,
	t_quote_iteri *iteri)
{
	t_arena	*arena_split_strings;

	arena_split_strings = &mini->arena_split_strings;
	if(!grow_arena_element(arena_split_strings, 1))
		return (1);
	iteri->field[iteri->split_count][iteri->str_pos] = s[iteri->i];
	iteri->str_pos++;
	iteri->i++;
	return (0);
}
