CC = cc
CFLAGS = -Werror -Wall -Wextra -g -fno-omit-frame-pointer -I. -I42_Libft -MMD -Wnull-dereference
LDFLAGS = -lreadline

ENVIRONMENT_CONVERSION = src/environment_conversion/convert_env_char_arr_to_lst.c \
						 src/environment_conversion/del_env_node_content.c \
						 src/environment_conversion/convert_env_lst_to_char_arr.c \
						 src/environment_conversion/get_env_from_lst.c \
						 src/environment_conversion/default_env.c

BUILTIN_FUNCTIONS = src/builtin_functions/cd_utils.c \
					src/builtin_functions/cd.c \
					src/builtin_functions/echo.c \
					src/builtin_functions/env.c \
					src/builtin_functions/exit.c \
					src/builtin_functions/export.c \
					src/builtin_functions/pwd.c \
					src/builtin_functions/unset.c

COMMAND_EXECUTION = src/command_execution/command_execution.c \
					src/command_execution/get_path_utils.c \
					src/command_execution/get_path.c \
					src/command_execution/is_builtin.c
 
FORK_MANAGEMENT = src/fork_management/redirections.c \
				  src/fork_management/parent.c \
				  src/fork_management/child_processes.c

CLEANUP_FUNCS = src/err_and_cleanup_funcs/close.c \
				src/err_and_cleanup_funcs/free_env_lst.c \
				src/err_and_cleanup_funcs/close_all_fds.c \
				src/err_and_cleanup_funcs/exec_err_msg.c \
				src/err_and_cleanup_funcs/error.c \
				src/err_and_cleanup_funcs/free_command_struct.c

EXEC_MAIN = src/exec_main.c

MAIN = main.c

EXECFILES = $(ENVIRONMENT_CONVERSION) $(BUILTIN_FUNCTIONS) $(COMMAND_EXECUTION) $(FORK_MANAGEMENT) $(EXEC_MAIN) $(CLEANUP_FUNCS)

START = src/minishell_start/inializing_minishell.c \
		src/minishell_start/readline.c \
		src/minishell_start/signals_help.c \
		src/minishell_start/signals.c

ARENA = src/arena/arena.c

TOKENIZATION =	src/tokenization/tokenization.c \
				src/tokenization/tokenization_operators.c \
				src/tokenization/tokenization_helpers.c

PARSING = src/parsing/parsing.c \
		  src/parsing/parsing_redirection.c \
		  src/parsing/parsing_word.c \
		  src/parsing/parsing_helpers.c

QRVE = src/qrve/quote_removal_var_expansion.c \
		src/qrve/var_expansion_dollar_found.c \
		src/qrve/find_var_and_expand.c \
		src/qrve/qrve_helpers.c

HEREDOC =	src/heredoc/heredoc.c \
			src/heredoc/heredoc_reading_writing1.c \
			src/heredoc/heredoc_reading_writing2.c \
			src/heredoc/heredoc_quote_removal.c \
			src/heredoc/heredoc_checker.c

CFILES = $(MAIN) $(START) $(EXECFILES) $(PARSING) $(ARENA) $(TOKENIZATION) $(HEREDOC) $(QRVE)
OFILES = $(CFILES:.c=.o)
DFILES = $(CFILES:.c=.d)

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
	$(MAKE) -C $(LIBFT)
	$(CC) $(CFLAGS) $(OFILES) $(LIBFT_A) $(LDFLAGS) -o $(NAME)

fclean: clean test_fclean
	$(REMOVE) $(NAME)
	$(MAKE) -C $(LIBFT) fclean

clean: test_clean
	$(REMOVE) $(OFILES)
	$(REMOVE) $(DFILES)
	$(MAKE) -C $(LIBFT) clean

re: fclean all

.PHONY: all clean fclean re test test_clean test_fclean