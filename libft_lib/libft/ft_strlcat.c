/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 16:09:42 by nildruon          #+#    #+#             */
/*   Updated: 2026/04/08 10:00:24 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	deststart;
	size_t	i;
	size_t	j;

	deststart = ft_strlen(dst);
	i = ft_strlen(dst);
	j = 0;
	if (size <= deststart)
		return (ft_strlen(src) + size);
	while (i < size -1 && src[j])
	{
		dst[i] = src[j];
		j++;
		i++;
	}
	dst[i] = '\0';
	return (deststart + ft_strlen(src));
}

/*
#include <bsd/string.h>
#include <stdio.h>
int main(void)
{
	char	dest1[21] = "Stefan";
	char	dest2[21] = "Stefan";
	char	*src1 = &dest1[2];
	char	*src2 = &dest2[2];
	size_t	size = 15;
	printf("Original Ret: %zu\n", strlcat(dest1, src1, size));
	printf("Original dest: %s\n", dest1);
	printf("Original src: %s\n", src1);
	printf("FT Ret: %zu\n", ft_strlcat(dest2, src2, size));
	printf("FT dest: %s\n", dest2);
	printf("FT src: %s\n", src2);
}*/
/*#include <stdio.h>
#include <bsd/string.h>

int    main(void)
{
    size_t    lenght;

    char    dest[] = "abcde";
    char    src[] = "1234\0aaaaaaaaaa\0";

    printf("ft_strlcat: \n");

    lenght = ft_strlcat(&src[4], src, 2);
    printf("%zu\n", lenght);
    printf("%s\n", src);

    char    dest1[] = "abcde";
    char    src1[] = "1234\0aaaaaaaaaa\0";

    printf("strlcat: \n");

    lenght = strlcat(&src1[4], src1, 2);
    printf("%zu\n", lenght);
    printf("%s\n", src1);
}*/
