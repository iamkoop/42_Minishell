/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 16:27:55 by bastalze          #+#    #+#             */
/*   Updated: 2026/05/31 17:21:49 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "../../minishell.h"

void	error(char *message)
{
	size_t	len;

	len = ft_strlen(message);
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, message, len);
	write(STDERR_FILENO, "\n", 1);
}
