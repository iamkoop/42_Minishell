/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 11:30:43 by nildruon          #+#    #+#             */
/*   Updated: 2026/04/09 15:40:20 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	search_in_remainder(t_buffer **rem)
{
	t_buffer	*tmp;
	int			i;

	if (!rem || !*rem)
		return (0);
	tmp = *rem;
	while (tmp)
	{
		i = tmp->index;
		while (tmp->buffer[i])
		{
			if (tmp->buffer[i] == '\n')
				return (1);
			i++;
		}
		tmp = tmp->next;
	}
	return (0);
}

void	remove_used(t_buffer **rem)
{
	t_buffer	*nxt;
	t_buffer	*curr;

	curr = *rem;
	while (curr && curr->used == 1)
	{
		nxt = curr->next;
		free(curr);
		curr = nxt;
	}
	*rem = curr;
}

t_buffer	*ft_gnl_lstnew(void)
{
	t_buffer	*new;

	new = malloc(sizeof(t_buffer));
	if (!new)
		return (NULL);
	new->index = 0;
	new->used = 0;
	new->next = NULL;
	return (new);
}

void	ft_gnl_lstadd(t_buffer **lst, t_buffer *new_node)
{
	t_buffer	*l;

	if (!lst || !new_node)
		return ;
	if (!*lst)
	{
		*lst = new_node;
		return ;
	}
	l = *lst;
	while (l->next)
		l = l->next;
	l->next = new_node;
}

void	ft_gnl_lstclear(t_buffer **lst)
{
	t_buffer	*nxt;
	t_buffer	*curr;

	if (!lst || !*lst)
		return ;
	curr = *lst;
	while (curr)
	{
		nxt = curr->next;
		free(curr);
		curr = nxt;
	}
	*lst = NULL;
}
