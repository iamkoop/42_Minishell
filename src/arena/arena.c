/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 17:33:35 by bastalze          #+#    #+#             */
/*   Updated: 2026/09/10 10:10:14 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_arena	arena_init(void)
{
	t_arena	arena;

	ft_memset(&arena, 0, sizeof(t_arena));
	arena.cap = ARENA_SIZE;
	return (arena);
}

void	*get_arena_element_start(t_arena *arena)
{
	void	*ptr;

	ptr = (void *)(arena->data + arena->pos);
	return (ptr);
}

bool	grow_arena_element(t_arena *arena, size_t size)
{
	if (arena->pos + size >= arena->cap)
	{
		error("exceeding memory limit: arena full");
		return (false);
	}
	arena->pos += size;
	return (true);
}

void	arena_init_all(t_minishell *mini)
{
	mini->arena_strings = arena_init();
	mini->arena_tokens = arena_init();
}
