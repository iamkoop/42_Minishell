/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 13:55:28 by nildruon          #+#    #+#             */
/*   Updated: 2026/04/09 15:40:34 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>

# ifndef BUFFER_SIZE

#  define BUFFER_SIZE 10000
# endif

typedef struct s_buffer
{
	char			buffer[BUFFER_SIZE +1];
	int				index;
	int				used;
	struct s_buffer	*next;
}					t_buffer;

char		*get_next_line(int fd);
t_buffer	*ft_gnl_lstnew(void);
void		ft_gnl_lstadd(t_buffer **lst, t_buffer *new_node);
void		ft_gnl_lstclear(t_buffer **lst);
void		remove_used(t_buffer **rem);
int			search_in_remainder(t_buffer **rem);
#endif
