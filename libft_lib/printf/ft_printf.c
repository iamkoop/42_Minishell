/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 21:07:04 by nildruon          #+#    #+#             */
/*   Updated: 2026/02/04 15:05:41 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	print_arg_helper(char c, va_list arg)
{
	char	*s;
	int		n;
	int		len;

	if (c == 'd' || c == 'i')
	{
		n = va_arg(arg, int);
		ft_putnbr_fd(n, 1);
		s = ft_itoa(n);
		len = ft_strlen(s);
		free(s);
		return (len);
	}
	else if (c == 's')
	{
		s = va_arg(arg, char *);
		if (!s)
			s = "(null)";
		ft_putstr_fd(s, 1);
		return (ft_strlen(s));
	}
	return (-1);
}

static int	print_arg(char *s, va_list arg)
{
	if (s[0] == 'c')
	{
		ft_putchar_fd(va_arg(arg, int), 1);
		return (1);
	}
	else if (s[0] == '%')
	{
		ft_putchar_fd('%', 1);
		return (1);
	}
	else if (s[0] == 'd' || s[0] == 'i' || s[0] == 's')
		return (print_arg_helper(s[0], arg));
	else if (s[0] == 'p')
		return (print_pointer((va_arg(arg, void *))));
	else if (s[0] == 'u')
		return (putnbr_u_int((va_arg(arg, unsigned int)), 0));
	else if (s[0] == 'x' || s[0] == 'X')
		return (print_hex(s[0], va_arg(arg, unsigned int), 0));
	return (-1);
}

static char	*format_specifier(const char	*c, int	*skipped_spaces)
{
	int		i;
	char	*set;

	*skipped_spaces = 1;
	i = 1;
	set = "cspdiuxX%";
	if (c[0] != '%')
		return (NULL);
	while (c[i] && c[i] == ' ')
	{
		(*skipped_spaces)++;
		i++;
	}
	if (ft_strchr(set, c[i]))
	{
		(*skipped_spaces)++;
		return ((char *)&c[i]);
	}
	return (NULL);
}

static int	print_formated(const char *format, va_list *arg)
{
	int		i;
	int		cnt;
	char	*specifier;
	int		skipped_spaces;

	skipped_spaces = 0;
	i = 0;
	cnt = 0;
	while (format[i])
	{
		specifier = format_specifier(&format[i], &skipped_spaces);
		if (!specifier)
		{
			write(1, &format[i++], 1);
			cnt++;
		}
		else
		{
			if (*specifier == '\0')
				return (-1);
			cnt += print_arg(specifier, *arg);
			i += skipped_spaces;
		}
	}
	return (cnt);
}

int	ft_printf(const char *format, ...)
{
	int		count_var;
	va_list	arg;

	if (!format)
		return (-1);
	va_start(arg, format);
	if (ft_strlen(format) == 0)
		count_var = 0;
	else
		count_var = print_formated(format, &arg);
	va_end(arg);
	return (count_var);
}

/*int main(void)
{
	int i = ft_printf("%d, %d\n", 1 , 88);
	int b = printf("%d, %d\n", 1 , 88);
	printf("%d\n", i);
	printf("%d\n", b);
}*/
