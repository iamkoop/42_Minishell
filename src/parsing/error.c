/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 16:27:55 by bastalze          #+#    #+#             */
/*   Updated: 2026/05/31 16:39:34 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include  "minishell.h"

void	error(char *message)
{
	size_t	len;

	len = ft_strlen(message);
	write(stderr, message_with_nl, len);
	write(stderr, '\n', 1);
}
