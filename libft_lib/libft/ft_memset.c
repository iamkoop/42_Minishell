/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 14:22:03 by nildruon          #+#    #+#             */
/*   Updated: 2025/10/16 21:27:47 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*ptr;

	i = 0;
	ptr = (unsigned char *)s;
	while (i < n)
	{
		ptr[i] = c;
		i++;
	}
	return (ptr);
}

/*
#include <stdio.h>
int main(void)
{
	unsigned char ptr[] = "Test"; 
	unsigned char *ptr2; 

	printf("%s\n", ptr);
	ptr2 = (unsigned char *) ft_memset(ptr,'P', 3);
	printf("%s\n", ptr2);
}*/
