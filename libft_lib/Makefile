# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/15 14:28:47 by nildruon          #+#    #+#              #
#    Updated: 2026/04/19 16:42:31 by nildruon         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Werror -Wall -Wextra -g -I.

LIBFT = libft/ft_isalnum.c libft/ft_isprint.c libft/ft_memcpy.c libft/ft_putendl_fd.c libft/ft_strchr.c libft/ft_strlcat.c libft/ft_strncmp.c libft/ft_substr.c \
libft/ft_atoi.c libft/ft_atol.c libft/ft_isalpha.c libft/ft_itoa.c libft/ft_memmove.c libft/ft_putnbr_fd.c libft/ft_strdup.c libft/ft_strlcpy.c libft/ft_strnstr.c libft/ft_tolower.c \
libft/ft_bzero.c libft/ft_isascii.c libft/ft_memchr.c libft/ft_memset.c libft/ft_putstr_fd.c libft/ft_striteri.c libft/ft_strlen.c libft/ft_strrchr.c libft/ft_toupper.c \
libft/ft_calloc.c libft/ft_isdigit.c libft/ft_memcmp.c libft/ft_putchar_fd.c libft/ft_split.c libft/ft_strjoin.c libft/ft_strjoin_three.c libft/ft_strmapi.c libft/ft_strtrim.c \
libft/ft_lstadd_back.c libft/ft_lstadd_front.c libft/ft_lstclear.c libft/ft_lstdelone.c libft/ft_lstnew.c libft/ft_lstnew.c libft/ft_lst_add.c\

PRINTF = printf/ft_printf.c printf/hex_converter.c printf/pointer_converter.c printf/putnbr_u_int.c

GNL = gnl/get_next_line.c gnl/get_next_line_utils.c

CFILES = $(LIBFT) $(PRINTF) $(GNL)


OFILES = $(CFILES:.c=.o)

NAME = libft.a

HEADER = libft.h

CREATE = ar rcs

REMOVE = rm -f


all: $(NAME)
$(NAME): $(HEADER) $(OFILES)
	$(CREATE) $(NAME) $?

fclean: clean
	$(REMOVE) $(NAME)

clean:
	$(REMOVE) $(OFILES)

re: fclean all

.PHONY: all clean fclean re