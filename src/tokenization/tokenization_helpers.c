/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 10:56:18 by bastalze          #+#    #+#             */
/*   Updated: 2026/08/20 21:33:42 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	start_first_token(t_minishell *mini, t_token_iteri *iteri);
int	add_to_token(char c, t_minishell *mini, t_token_iteri *iteri);
int	delimit_token(t_minishell *mini, t_token_iteri *iteri);

int	add_to_token(char c, t_minishell *mini, t_token_iteri *iteri)
{
	t_arena	*arena_strings;

	arena_strings = &mini->arena_strings;
	if (!grow_arena_element(arena_strings, 1))
		return (mini->exit_status = 1, 1);
	iteri->tok->token_str[iteri->str_pos] = c;
	iteri->str_pos++;
	return (0);
}

int	delimit_token(t_minishell *mini, t_token_iteri *iteri)
{
	t_arena	*arena_strings;
	t_arena	*arena_tokens;

	arena_strings = &mini->arena_strings;
	arena_tokens = &mini->arena_tokens;
	if (!grow_arena_element(arena_strings, 1))
		return (mini->exit_status = 1, 1);
	iteri->tok = get_arena_element_start(arena_tokens);
	if (!grow_arena_element(arena_tokens, sizeof(t_token_node)))
		return (mini->exit_status = 1, 1);
	iteri->tok->token_str = get_arena_element_start(arena_strings);
	iteri->str_pos = 0;
	return (0);
}

int	start_first_token(t_minishell *mini, t_token_iteri *iteri)
{
	t_arena	*arena_strings;
	t_arena	*arena_tokens;

	arena_strings = &mini->arena_strings;
	arena_tokens = &mini->arena_tokens;
	iteri->tok = get_arena_element_start(arena_tokens);
	if (!grow_arena_element(arena_tokens, sizeof(t_token_node)))
		return (mini->exit_status = 1, 1);
	iteri->tok->token_str = get_arena_element_start(arena_strings);
	iteri->str_pos = 0;
	return (0);
}
