CC = cc
CFLAGS = -Werror -Wall -Wextra -g -I42_Libft

CFILES = convert_envp_to_lst.c test_command_exec.c

OFILES = $(CFILES:.c=.o)


NAME = minishell

HEADER = minishell.h

LIBFT = 42_Libft
LIBFT_A = $(LIBFT)/libft.a
CREATE = ar rcs

REMOVE = rm -f


%.o: %.c  $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OFILES)
	make -C $(LIBFT)
	$(CC) $(CFLAGS) $(OFILES) $(LIBFT_A) $(FDFFLAGS) -o $(NAME)

fclean: clean
	$(REMOVE) $(NAME)
	@make -C $(LIBFT) fclean

clean:
	$(REMOVE) $(OFILES)
	@make -C $(LIBFT) clean

re: fclean all

.PHONY: all clean fclean re