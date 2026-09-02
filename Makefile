CC = cc
CFLAGS = -Werror -Wall -Wextra -g -fno-omit-frame-pointer -I -I42_Libft -MMD -Wnull-dereference
LDFLAGS = -lreadline

ENVIRONMENT_CONVERSION = src/environment_conversion/convert_env_char_arr_to_lst.c \
						 src/environment_conversion/del_env_node_content.c \
						 src/environment_conversion/convert_env_lst_to_char_arr.c \
						 src/environment_conversion/get_env_from_lst.c \
						 src/environment_conversion/default_env.c

BUILTIN_FUNCTIONS = src/builtin_functions/cd.c \
					src/builtin_functions/echo.c \
					src/builtin_functions/env.c \
					src/builtin_functions/exit.c \
					src/builtin_functions/export.c \
					src/builtin_functions/pwd.c \
					src/builtin_functions/unset.c

COMMAND_EXECUTION = src/command_execution/command_execution.c \
					src/command_execution/get_path.c \
					src/command_execution/is_builtin.c
 
FORK_MANAGEMENT = src/fork_management/redirections.c \
				  src/fork_management/parent.c \
				  src/fork_management/child_processes.c

CLEANUP_FUNCS = src/err_and_cleanup_funcs/close.c \
				src/err_and_cleanup_funcs/free_env_lst.c \
				src/err_and_cleanup_funcs/close_all_fds.c \
				src/err_and_cleanup_funcs/exec_err_msg.c \
				src/err_and_cleanup_funcs/free_all.c \
				src/err_and_cleanup_funcs/close_heredoc_fds.c

EXEC_MAIN = src/exec_main.c

ARENA = src/arena/arena.c

MAIN = main.c

EXECFILES = $(ENVIRONMENT_CONVERSION) $(BUILTIN_FUNCTIONS) $(COMMAND_EXECUTION) $(FORK_MANAGEMENT) $(EXEC_MAIN) $(CLEANUP_FUNCS)

START = src/minishell_start/inializing_minishell.c \
		src/minishell_start/readline.c

PARSING = src/parsing/tokenization.c \
		  src/parsing/tokenization_operators.c \
		  src/parsing/tokenization_helpers.c \
		  src/parsing/heredoc.c \
		  src/parsing/heredoc_reading_writing.c \
		  src/parsing/heredoc_quote_removal.c \
		  src/parsing/error.c \
		  src/parsing/delete_heredocs.c \
		  src/parsing/free_command_struct.c \
		  src/parsing/parsing.c \
		  src/parsing/parsing_redirection.c \
		  src/parsing/parsing_word.c \
		  src/parsing/parsing_helpers.c \
		  src/parsing/quote_removal_var_expansion.c \
		  src/parsing/var_expansion_dollar_found.c \
		  src/parsing/find_var_and_expand.c \
		  src/parsing/qrve_helpers.c

CFILES = $(MAIN) $(START) $(EXECFILES) $(PARSING) $(ARENA)
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
	make -C $(LIBFT)
	$(CC) $(CFLAGS) $(OFILES) $(LIBFT_A) $(LDFLAGS) -o $(NAME)

fclean: clean test_fclean
	$(REMOVE) $(NAME)
	@make -C $(LIBFT) fclean

clean: test_clean
	$(REMOVE) $(OFILES)
	$(REMOVE) $(DFILES)
	@make -C $(LIBFT) clean

re: fclean all

.PHONY: all clean fclean re test test_clean test_fclean

# ============================================================================ #
#  TESTING CONFIGURATION (DELETE / COMMENT THIS ENTIRE BLOCK FOR DEFENSE)      #
# ============================================================================ #

TEST_NAME   = test
TEST_FLAGS  = -Itesting -DTESTING=1

# 1. DEEP SEARCH: Finds execution tests and main_testing.c inside testing/
TEST_SRCS   = $(shell find testing/execution_tests -name "*.c") testing/main_testing.c
TEST_OFILES = $(TEST_SRCS:.c=.o)

# 2. UNIVERSAL RULE: Compiles ANY .o file located anywhere inside the testing/ tree.
# (21.6.2026 Barbara removed $(HEADER) to make the testing.h file work)
$(TEST_OFILES): %.o: %.c testing/testing.h $(HEADER)
	$(CC) $(CFLAGS) $(TEST_FLAGS) -c $< -o $@

# Test execution target
test: $(OFILES) $(TEST_OFILES)
	make -C $(LIBFT)
	$(CC) $(CFLAGS) $(TEST_FLAGS) $(OFILES) $(TEST_OFILES) $(LIBFT_A) $(LDFLAGS) -o $(TEST_NAME)

# Dedicated cleanup rules for the testing suite
test_clean:
	$(REMOVE) $(TEST_OFILES)

test_fclean:
	$(REMOVE) $(TEST_NAME)
