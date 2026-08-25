/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_var_and_expand.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 12:45:00 by bastalze          #+#    #+#             */
/*   Updated: 2026/08/20 21:34:10 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int			find_var(char *var, t_minishell *mini,
				t_quote_iteri *iteri, t_single_linked_node *env);
static int	variable_expansion(t_minishell *mini,
				t_quote_iteri *iteri, t_env_var *tmp_content);
static int	field_splitting(t_minishell *mini,
				t_quote_iteri *iteri);
static int	add_variable_char(t_minishell *mini,
				t_quote_iteri *iteri, int i, t_env_var *tmp_content);

int	find_var(char *var, t_minishell *mini,
		t_quote_iteri *iteri, t_single_linked_node *env)
{
	t_single_linked_node	*tmp_node;
	t_env_var				*tmp_content;

	tmp_node = env;
	while (tmp_node)
	{
		tmp_content = (t_env_var *)tmp_node->content;
		if (!ft_strncmp(tmp_content->key, var, ft_strlen(var)))
		{
			if (variable_expansion(mini, iteri, tmp_content))
				return (1);
		}
		tmp_node = tmp_node->next;
	}
	return (0);
}

static int	variable_expansion(t_minishell *mini,
		t_quote_iteri *iteri, t_env_var *tmp_content)
{
	int	i;

	i = 0;
	while (tmp_content->value && tmp_content->value[i])
	{
		if ((tmp_content->value[i] == ' ' || tmp_content->value[i] == '\t'
			|| tmp_content->value[i] == '\n') && iteri->quoted == false
			&& iteri->heredoc == false)
		{
			if (iteri->has_char == true)
			{
				if (field_splitting(mini, iteri))
					return (1);
			}
			iteri->has_char = false;
		}
		else
		{
			if (add_variable_char(mini, iteri, i, tmp_content))
				return (1);
			iteri->has_char = true;
		}
		i++;
	}
	return (0);
}

static int	field_splitting(t_minishell *mini,
		t_quote_iteri *iteri)
{
	t_arena	*arena_split_tokens;
	t_arena	*arena_split_strings;

	arena_split_tokens = &mini->arena_split_tokens;
	arena_split_strings = &mini->arena_split_strings;
	add_to_word(0, mini, iteri);
	iteri->field = get_arena_element_start(arena_split_tokens);
	if(!grow_arena_element(arena_split_tokens, sizeof(char *)))
		return (1);
	iteri->field[0] = get_arena_element_start(arena_split_strings);
	iteri->split_count++;
	iteri->str_pos = 0;
	return (0);
}

//iteri->field = get_arena_element_start(arena_split_tokens);
	//if(!grow_arena_element(arena_split_tokens, sizeof(char *)))
	//	return (1);
	//iteri->field[iteri->split_count] = get_arena_element_start(arena_split_strings);
	//iteri->split_count++;

static int	add_variable_char(t_minishell *mini,
		t_quote_iteri *iteri, int i, t_env_var *tmp_content)
{
	t_arena	*arena_split_strings;

	arena_split_strings = &mini->arena_split_strings;
	if(!grow_arena_element(arena_split_strings, 1))
		return (1);
	iteri->field[0][iteri->str_pos] = tmp_content->value[i];
	iteri->str_pos++;
	return (0);
}
