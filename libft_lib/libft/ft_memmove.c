/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 17:34:18 by nildruon          #+#    #+#             */
/*   Updated: 2025/10/16 21:24:37 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;
	size_t			i;

	d = (unsigned char *)dest;
	s = (unsigned char *)src;
	i = 0;
	if (dest > src)
	{
		while (n-- > 0)
			d[n] = s[n];
	}
	else
	{
		while (i < n)
		{
			d[i] = s[i];
			i++;
		}
	}
	return (dest);
}

/*
#include <stdio.h>
int main(void)
{
	unsigned char src[] = "Test";
	unsigned char dest[] = "42vienna";

	unsigned char over[] = "42vienna";
	unsigned char over2[] = "42vienna";

	printf("Normal");

	printf("%s\n", src);
	printf("%s\n", dest);
	ft_memmove(dest, src, 3);
	printf("%s\n", src);
	printf("%s\n", dest);

	printf("Overlap\n");

	printf("%s\n", over);
	ft_memmove(over + 3, over, 8);
	printf("%s\n", over);

	printf("Overlap2\n");
	printf("%s\n", over2);
	ft_memmove(over2, over2 +3, 8);
	printf("%s\n", over2);
}*/
