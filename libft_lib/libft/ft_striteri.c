/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 11:20:49 by nildruon          #+#    #+#             */
/*   Updated: 2025/10/16 21:30:21 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	int		i;

	i = 0;
	if (!s || !f)
		return ;
	while (s[i])
	{
		f(i, &s[i]);
		i++;
	}
}
/*
#include <stdio.h>

void	toupperRevamped(unsigned int i, char *s)
{
	if (s[i] >= 'a' && s[i] <= 'z')
		s[i] -= 32;
}

int main(void)
{
	char s1[] = "cats and dogs";
	printf("%s\n", s1);
	ft_striteri(s1, toupperRevamped);
	printf("%s\n", s1);
}*/