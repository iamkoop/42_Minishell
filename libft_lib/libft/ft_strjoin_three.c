/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_three.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 11:59:29 by nildruon          #+#    #+#             */
/*   Updated: 2026/04/23 17:19:34 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_three(char const *s1, char const *s2, char const *s3)
{
	size_t	s1_len;
	size_t	s2_len;
	size_t	s3_len;
	char	*concat;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	s3_len = 0;
	if (s3)
		s3_len = ft_strlen(s3);
	concat = (char *)malloc(s1_len + s2_len + s3_len + 1);
	if (!concat)
		return (NULL);
	ft_strlcpy(concat, s1, s1_len + 1);
	ft_strlcat(concat, s2, s2_len + s1_len + 1);
	if (s3)
		ft_strlcat(concat, s3, s3_len + s2_len + s1_len + 1);
	return (concat);
}
