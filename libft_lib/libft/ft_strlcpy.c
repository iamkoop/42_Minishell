/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 11:13:41 by nildruon          #+#    #+#             */
/*   Updated: 2026/03/19 11:43:59 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size == 0)
		return (ft_strlen(src));
	while (i < size -1 && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen(src));
}

/*
#include <stdio.h>
#include <bsd/string.h>
int main(void)
{
	char dst[] = "mabe1234";
	char *src = &dst[2];
	printf("%s\n", dst);
	printf("%s\n", src);
	printf("%zu\n", ft_strlcpy(dst, src, 3));
	printf("%s\n", dst);
	printf("%s\n", src);

	char original[] = "Stefi";
	char *src = &original[0];
	char *dst = &original[0];
	char *src2 = &original[0];
	char *dst2 = &original[0];
	printf("%s\n", dst);
	printf("%s\n", src);
	printf("%zu\n", ft_strlcpy(dst, src, 0));
	printf("%zu\n", strlcpy(dst2, src2, 0));
	printf("%s\n", dst);
	printf("%s\n", src);
	printf("%s\n", dst2);
	printf("%s\n", src2);
}*/
