/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:13:42 by nildruon          #+#    #+#             */
/*   Updated: 2025/10/16 21:35:16 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

//casted the c cuz of edgecase 't' +256

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	int		foundlast;
	char	*last;

	i = 0;
	foundlast = 0;
	last = NULL;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
		{
			last = (char *)&s[i];
			foundlast = 1;
		}
		i++;
	}
	if (c == '\0')
		return ((char *)&s[i]);
	if (foundlast)
		return (last);
	return (0);
}

/*int	main(void)
{
	printf("%s\n", ft_strrchr("Hello world", '\0'));
	printf("%s\n", ft_strrchr("Do you remember", 'e'));
	printf("%s\n", ft_strrchr("Hello world", ' '));
}*/