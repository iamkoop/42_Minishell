/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 16:27:55 by bastalze          #+#    #+#             */
/*   Updated: 2026/08/20 21:34:07 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	error(char *message)
{
	size_t	len;

	len = ft_strlen(message);
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, message, len);
	write(STDERR_FILENO, "\n", 1);
}
