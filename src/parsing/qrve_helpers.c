/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   qrve_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 17:34:37 by bastalze          #+#    #+#             */
/*   Updated: 2026/08/24 17:34:40 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void init_qrve_arena(t_minishell *mini)
{
	mini->arena_split_tokens = arena_init();
	mini->arena_split_strings = arena_init();
}

int	start_first_word(t_minishell *mini, t_quote_iteri *iteri)
{
	t_arena	*arena_split_tokens;
	t_arena	*arena_split_strings;

	arena_split_strings = &mini->arena_split_strings;
	arena_split_tokens = &mini->arena_split_tokens;
	iteri->field = get_arena_element_start(arena_split_tokens);
	if(!grow_arena_element(arena_split_tokens, sizeof(char *)))
		return (mini->exit_status = 1, 1);
	iteri->field[0] = get_arena_element_start(arena_split_strings);
	// iteri->split_count = 1;
	iteri->str_pos = 0;
	return (0);
}

int	add_to_word(char c, t_minishell *mini, t_quote_iteri *iteri)
{
	t_arena	*arena_split_strings;

	arena_split_strings = &mini->arena_split_strings;
	if(!grow_arena_element(arena_split_strings, 1))
		return (mini->exit_status = 1, 1);
	iteri->field[0][iteri->str_pos] = c;
	iteri->str_pos++;
	return (0);
}
/*
int	delimit_word_array(t_minishell *mini, t_quote_iteri *iteri)
{
	t_arena	*arena_split_tokens;
	t_arena	*arena_split_strings;

	arena_split_tokens = &mini->arena_split_tokens;
	arena_split_strings = &mini->arena_split_strings;
	if(!grow_arena_element(arena_split_strings, 1))
		return (1);
	iteri->field = get_arena_element_start(arena_split_tokens);
	if(!grow_arena_element(arena_split_tokens, sizeof(char *)))
		return (1);
	iteri->split_count++;
	iteri->field[iteri->split_count] = get_arena_element_start(arena_split_strings);
	iteri->str_pos = 0;
	return (0);
}
*/

