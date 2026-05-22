CC = cc
CFLAGS = -Werror -Wall -Wextra -g -I42_Libft

EXECFILES = convert_envp_to_lst.c test_command_exec.c
CFILES = $(EXECFILES)
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

fclean: clean test_fclean
	$(REMOVE) $(NAME)
	@make -C $(LIBFT) fclean

clean: test_clean
	$(REMOVE) $(OFILES)
	@make -C $(LIBFT) clean

re: fclean all

.PHONY: all clean fclean re test test_clean test_fclean

# ============================================================================ #
#  TESTING CONFIGURATION (DELETE / COMMENT THIS ENTIRE BLOCK FOR DEFENSE)     #
# ============================================================================ #

TEST_NAME   = test
TEST_FLAGS  = -Itesting

# 1. DEEP SEARCH: This recursively finds EVERY .c file inside testing/ and all subfolders
TEST_SRCS   = $(shell find testing -name "*.c")
TEST_OFILES = $(TEST_SRCS:.c=.o)

# 2. UNIVERSAL RULE: Compiles ANY .o file located anywhere inside the testing/ tree.
$(TEST_OFILES): %.o: %.c testing/testing.h $(HEADER)
	$(CC) $(CFLAGS) $(TEST_FLAGS) -c $< -o $@

# Test execution target
test: $(OFILES) $(TEST_OFILES)
	make -C $(LIBFT)
	$(CC) $(CFLAGS) $(TEST_FLAGS) $(OFILES) $(TEST_OFILES) $(LIBFT_A) -o $(TEST_NAME)

# Dedicated cleanup rules for the testing suite
test_clean:
	$(REMOVE) $(TEST_OFILES)

test_fclean:
	$(REMOVE) $(TEST_NAME)