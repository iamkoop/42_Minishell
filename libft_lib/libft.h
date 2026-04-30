/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:21:27 by nildruon          #+#    #+#             */
/*   Updated: 2026/04/19 18:21:58 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdarg.h>
# include <stdint.h>
# include <stdlib.h>
# include <unistd.h>
# include "gnl/get_next_line.h"
// Libft
typedef struct s_stack
{
	int				data;
	int				index;
	int				curr_i;
	struct s_stack	*next;
	struct s_stack	*prev;
}					t_stack;

typedef struct s_stacks
{
	int				len_a;
	int				len_b;
	t_stack			*stack_a;
	t_stack			*stack_b;
}					t_stacks;

int					ft_atoi(const char *nptr);
long				ft_atol(const char *nptr);
void				ft_bzero(void *s, size_t n);
void				*ft_calloc(size_t nmemb, size_t size);
int					ft_isalnum(int c);
int					ft_isalpha(int c);
int					ft_isascii(int c);
int					ft_isdigit(int c);
int					ft_isprint(int c);
char				*ft_itoa(int n);
void				*ft_memchr(const void *s, int c, size_t n);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
void				*ft_memcpy(void *dest, const void *src, size_t n);
void				*ft_memmove(void *dest, const void *src, size_t n);
void				*ft_memset(void *s, int c, size_t n);
void				ft_putchar_fd(char c, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);
void				ft_putstr_fd(char *s, int fd);
char				**ft_split(char const *s, char c);
char				*ft_strchr(const char *s, int c);
char				*ft_strdup(const char *s);
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strjoin_three(char const *s1, char const *s2,
						char const *s3);
size_t				ft_strlcat(char *dst, const char *src, size_t size);
size_t				ft_strlcpy(char *dst, const char *src, size_t size);
size_t				ft_strlen(const char *s);
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
int					ft_strncmp(const char *s1, const char *s2, size_t n);
char				*ft_strnstr(const char *big, const char *little,
						size_t len);
char				*ft_strrchr(const char *s, int c);
char				*ft_strtrim(char const *s1, char const *set);
char				*ft_substr(char const *s, unsigned int start, size_t len);
int					ft_tolower(int c);
int					ft_toupper(int c);
t_stack				*ft_lstnew(int content, int index);
void				ft_lstadd(t_stack **lst, t_stack *new_node);
// void		ft_lstadd_front(t_stack **lst, t_stack *new_node);
int					ft_lstsize(t_stack *lst);
t_stack				*ft_lstlast(t_stack *lst);
// void		ft_lstadd_back(t_stack **lst, t_stack *new_node);
//void				ft_lstdelone(t_stack *lst, void (*del)(void *));
void				ft_lstclear(t_stack **lst);
// Printf
int					print_hex(char c, uintptr_t nb, int len);
int					print_pointer(void *ptr);
int					putnbr_u_int(unsigned int n, int len);
int					ft_printf(const char *format, ...);

#endif
