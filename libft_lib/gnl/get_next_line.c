/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 11:30:43 by nildruon          #+#    #+#             */
/*   Updated: 2026/04/10 21:04:27 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	line_help(t_buffer **rem)
{
	t_buffer	*stack;
	int			i;
	int			cnt;

	cnt = 0;
	stack = *rem;
	while (stack)
	{
		i = stack->index;
		while (stack->buffer[i])
		{
			cnt++;
			if (stack->buffer[i] == '\n')
				return (cnt);
			i++;
		}
		stack = stack->next;
	}
	return (cnt);
}

static char	*line(t_buffer	**rem, int full_ret_len)
{
	t_buffer	*stack;
	char		*str;
	int			cnt;

	if (!rem || !*rem)
		return (NULL);
	full_ret_len = line_help(rem);
	str = malloc(full_ret_len + 1);
	if (!str)
		return (NULL);
	cnt = 0;
	stack = *rem;
	while (stack && cnt < full_ret_len)
	{
		while (stack->buffer[stack->index] && cnt < full_ret_len)
			str[cnt++] = stack->buffer[stack->index++];
		if (stack->buffer[stack->index] == '\0')
			stack->used = 1;
		stack = stack->next;
	}
	str[cnt] = '\0';
	return (str);
}

static int	scan_for_nl(char	*s, int r)
{
	int	i;

	i = 0;
	while (i < r)
	{
		if (s[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

static int	read_buffer(int fd, t_buffer	**remainder, int	*nl)
{
	int			r;
	t_buffer	*node;

	node = ft_gnl_lstnew();
	if (!node)
		return (-1);
	r = read(fd, node->buffer, BUFFER_SIZE);
	if (r == 0)
		return (free(node), 0);
	if (r == -1)
		return (free(node), -1);
	node->buffer[r] = '\0';
	*nl = scan_for_nl(node->buffer, r);
	if (!node)
		return (-1);
	if (!remainder || !*remainder)
		*remainder = node;
	else
		ft_gnl_lstadd(remainder, node);
	return (r);
}

char	*get_next_line(int fd)
{
	static t_buffer	*remainder;
	int				r;
	int				nl_found;
	char			*ret_line;

	r = 1;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (ft_gnl_lstclear(&remainder), NULL);
	nl_found = 0;
	if (search_in_remainder(&remainder))
		nl_found = 1;
	while (r > 0)
	{
		if (r == 0 || nl_found)
			break ;
		r = read_buffer(fd, &remainder, &nl_found);
		if (r == -1)
			return (ft_gnl_lstclear(&remainder), NULL);
	}
	ret_line = line(&remainder, 0);
	remove_used(&remainder);
	return (ret_line);
}
