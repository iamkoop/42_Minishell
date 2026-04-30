/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 17:20:50 by nildruon          #+#    #+#             */
/*   Updated: 2025/10/16 21:24:12 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;
	size_t			i;

	d = (unsigned char *)dest;
	s = (unsigned char *)src;
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}

/*
#include <stdio.h>
int main(void)
{
	unsigned char src[] = "42Marvin";
	unsigned char dest[] = "42vienna";

	printf("Normal\n");

	printf("%s\n", src);
	printf("%s\n", dest);
	ft_memcpy(dest, src, 8);
	printf("%s\n", src);
	printf("%s\n", dest);
}*/
