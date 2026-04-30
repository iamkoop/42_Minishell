/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pointer_converter.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 21:01:06 by nildruon          #+#    #+#             */
/*   Updated: 2026/02/04 15:06:32 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	print_pointer(void *ptr)
{
	uintptr_t	nb;
	int			len;

	if (!ptr)
	{
		ft_putstr_fd("(nil)", 1);
		return (5);
	}
	nb = (uintptr_t)ptr;
	ft_putstr_fd("0x", 1);
	len = 2 + print_hex('x', nb, 0);
	return (len);
}
