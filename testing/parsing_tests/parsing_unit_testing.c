/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_unit_testing.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/29 11:13:01 by bastalze          #+#    #+#             */
/*   Updated: 2026/07/30 14:28:23 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../minishell.h"
#include "../testing.h"

typedef struct s_test_case
{
    char                **tokens;          // Raw token strings
    enum e_token_type   *token_types;      // Their types
    char                **expected_argv;   // Expected argv
    char                **expected_filenames; // Expected filenames
    int                 *expected_fds;     // Expected fds
    enum e_redir_type   *expected_redir_types; // Expected redir types
    int                 expected_return;
	int					expected_redirection_count;
	char				*test_case_name;
} t_test_case;

// Comparison functions
static int compare_argv(t_command *cmd_content, char **expected_argv)
{
    int i = 0;
    int errors = 0;
    
    if (!cmd_content->argv && !expected_argv)
        return 0;
    if (!cmd_content->argv || !expected_argv)
    {
        printf("ERROR: One argv is NULL, other is not\n");
        return 1;
    }
    while (cmd_content->argv[i] || expected_argv[i])
    {
        if (!cmd_content->argv[i])
        {
            printf("ERROR: Missing argv[%d], expected '%s'\n", i, expected_argv[i]);
            errors++;
        }
        else if (!expected_argv[i])
        {
            printf("ERROR: Extra argv[%d] '%s', expected NULL\n", i, cmd_content->argv[i]);
            errors++;
        }
        else if (strcmp(expected_argv[i], cmd_content->argv[i]) != 0)
        {
            printf("ERROR: argv[%d] mismatch: expected '%s', got '%s'\n",
                   i, expected_argv[i], cmd_content->argv[i]);
            errors++;
        }
        i++;
    }
    return errors;
}

static int compare_redir(t_command *cmd_content, char **expected_filenames,
                         int *expected_fds, enum e_redir_type *expected_types,
						int expected_count)
{
    t_single_linked_node *curr = cmd_content->redir;
    t_redir_list *redir_content;
    int i = 0;
    int errors = 0;
    
    // Count actual redirections
    int actual_count = 0;
    t_single_linked_node *temp = curr;
    while (temp)
    {
        actual_count++;
        temp = temp->next;
    }
    
    if (actual_count != expected_count)
    {
        printf("ERROR: Redirection count mismatch: expected %d, got %d\n",
               expected_count, actual_count);
        return 1;
    }
    
    // Compare each redirection in order
    curr = cmd_content->redir;
    i = 0;
    while (curr)
    {
        redir_content = (t_redir_list *)curr->content;
        if (!expected_filenames[i])
		{
			if (redir_content->filename)
			{
				printf("ERROR: Redir[%d] filename mismatch: expected '', got '%s'\n",
						i, redir_content->filename);
				errors++;
			}
		}
		else
		{
			if (strcmp(expected_filenames[i], redir_content->filename) != 0)
			{
				printf("ERROR: Redir[%d] filename mismatch: expected '%s', got '%s'\n",
						i, expected_filenames[i], redir_content->filename);
				errors++;
			}
		}
        
        if (expected_fds[i] != redir_content->fd)
        {
            printf("ERROR: Redir[%d] fd mismatch: expected %d, got %d\n",
                   i, expected_fds[i], redir_content->fd);
            errors++;
        }
        
        if (expected_types[i] != redir_content->redir_type)
        {
            printf("ERROR: Redir[%d] type mismatch: expected %d, got %d\n",
                   i, expected_types[i], redir_content->redir_type);
            errors++;
        }
        
        curr = curr->next;
        i++;
    }
    return errors;
}

static int compare_command(t_command *cmd_content, t_test_case *tc)
{
    int errors = 0;
    
    errors += compare_argv(cmd_content, tc->expected_argv);
    errors += compare_redir(cmd_content, tc->expected_filenames,
                            tc->expected_fds, tc->expected_redir_types,
							tc->expected_redirection_count);
    return errors;
}

static void	build_token_list(char *tokens[], enum e_token_type token_types[], t_token_node *token_lst)
{
	int i = 0;

	while (tokens[i])
	{
		ft_strlcpy(token_lst[i].token_str, tokens[i], TOKEN_STR_SIZE);
		token_lst[i].token_type = token_types[i];
		i++;
	}
}

static int	init_cmd_data(t_cmd_data *cmd_data)
{
	t_command	*cmd;

	cmd = ft_calloc(1, sizeof(t_command));
	if(!cmd)
		return (1);
	cmd_data->head = ft_single_lstnew(cmd);
	if(!cmd_data->head)
		return (1);
	cmd_data->tail = cmd_data->head;
	return(0);
}

int run_single_command_test(t_test_case *tc)
{
    int				result;
    int				errors = 0;
    t_token_iteri	iteri;
    t_cmd_data		cmd_data;
	t_token_node	token_lst[TOKEN_AMOUNT];

	ft_bzero(token_lst, sizeof(t_token_node) * TOKEN_AMOUNT);
	ft_bzero(&iteri, sizeof(t_token_iteri));
    
    // Build token list from test case
    build_token_list(tc->tokens, tc->token_types, token_lst);
    
	printf("Test \"%s\":\n", tc->test_case_name);

    // Initialize cmd_data
    if (init_cmd_data(&cmd_data))
		return (1);
    
    // Call parsing
    result = parsing(NULL, token_lst, &iteri, &cmd_data);
    
    // Check return value
    if (result != tc->expected_return)
    {
        printf("ERROR: Return value mismatch: expected %d, got %d\n",
               tc->expected_return, result);
        errors++;
    }
    
    // If we expected success, compare the command struct
    if (tc->expected_return == 0 && cmd_data.head)
    {
        t_command *cmd_content = (t_command *)cmd_data.head->content;
        errors += compare_command(cmd_content, tc);
    }
    
    free_command_struct(&cmd_data);
    return errors;
}

int parsing_unit_testing()
{
	t_test_case tests[] = {
		// Test 1: Simple command "ls"
		{
			.tokens = (char *[]){"ls", NULL},
			.token_types = (enum e_token_type[]){WORD, DEFAULT},
			.expected_argv = (char *[]){"ls", NULL},
			.expected_filenames = (char *[]){NULL},
			.expected_fds = (int []){-1},
			.expected_redir_types = (enum e_redir_type[]){0},
			.expected_return = 0,
			.expected_redirection_count = 0,
			.test_case_name = "Simple command 'ls'"
		},
		// Test 2: "ls < file.txt"
		{
			.tokens = (char *[]){"ls", "<", "file.txt", NULL},
			.token_types = (enum e_token_type[]){WORD, REDIR_IN, WORD, DEFAULT},
			.expected_argv = (char *[]){"ls", NULL},
			.expected_filenames = (char *[]){"file.txt", NULL},
			.expected_fds = (int []){0},
			.expected_redir_types = (enum e_redir_type[]){IN},
			.expected_return = 0,
			.expected_redirection_count = 1,
			.test_case_name = "ls < file.txt"
		},
		// Test 3: Multiple redirections "ls < infile > outfile"
		{
			.tokens = (char *[]){"ls", "<", "infile", ">", "outfile", NULL},
			.token_types = (enum e_token_type[]){WORD, REDIR_IN, WORD, REDIR_OUT, WORD, DEFAULT},
			.expected_argv = (char *[]){"ls", NULL},
			.expected_filenames = (char *[]){"infile", "outfile", NULL},
			.expected_fds = (int []){0, 0},
			.expected_redir_types = (enum e_redir_type[]){IN, OUT},
			.expected_return = 0,
			.expected_redirection_count = 2,
			.test_case_name = "Multiple redirections 'ls < infile > outfile'"
		},
		// Test 4: "cat < infile >> outfile"
		{
			.tokens = (char *[]){"cat", "<", "infile", ">>", "outfile", NULL},
			.token_types = (enum e_token_type[]){WORD, REDIR_IN, WORD, REDIR_OUT_A, WORD, DEFAULT},
			.expected_argv = (char *[]){"cat", NULL},
			.expected_filenames = (char *[]){"infile", "outfile", NULL},
			.expected_fds = (int []){0, 0},
			.expected_redir_types = (enum e_redir_type[]){IN, APPEND},
			.expected_return = 0,
			.expected_redirection_count = 2,
			.test_case_name = "cat < infile >> outfile"
		},
		// Test 5: Error case - missing filename after redirection
		{
			.tokens = (char *[]){"ls", "<", NULL},
			.token_types = (enum e_token_type[]){WORD, REDIR_IN, DEFAULT},
			.expected_argv = (char *[]){NULL},
			.expected_filenames = (char *[]){NULL},
			.expected_fds = (int []){0},
			.expected_redir_types = (enum e_redir_type[]){0},
			.expected_return = 1,  // Expect some error code
			.expected_redirection_count = 0,
			.test_case_name = "Error case - missing filename after redirection"
		},
		// Test 6: Split command
		{
			.tokens = (char *[]){"ls", "<", "infile", "-l", NULL},
			.token_types = (enum e_token_type[]){WORD, REDIR_IN, WORD, WORD},
			.expected_argv = (char *[]){"ls", "-l", NULL},
			.expected_filenames = (char *[]){"infile"},
			.expected_fds = (int []){0},
			.expected_redir_types = (enum e_redir_type[]){IN},
			.expected_return = 0,
			.expected_redirection_count = 1,
			.test_case_name = "Split command"
		},
		// Test 7: Heredoc
		{
			.tokens = (char *[]){"cat", "<<", "4", NULL},
			.token_types = (enum e_token_type[]){WORD, HERE_DOC, WORD},
			.expected_argv = (char *[]){"cat", NULL},
			.expected_filenames = (char *[]){NULL},
			.expected_fds = (int []){4},
			.expected_redir_types = (enum e_redir_type[]){HERE},
			.expected_return = 0,
			.expected_redirection_count = 1,
			.test_case_name = "Heredoc"
		},
		// Test 8: Error case - double redirection < >
		{
			.tokens = (char *[]){"ls", "<", ">", NULL},
			.token_types = (enum e_token_type[]){WORD, REDIR_IN, REDIR_OUT},
			.expected_argv = (char *[]){NULL},
			.expected_filenames = (char *[]){NULL},
			.expected_fds = (int []){0},
			.expected_redir_types = (enum e_redir_type[]){0},
			.expected_return = 1,
			.expected_redirection_count = 0,
			.test_case_name = "Error case - double redirection < >"
		},
		// Test 9: Error case - double redirection > <
		{
			.tokens = (char *[]){"ls", ">", "<", NULL},
			.token_types = (enum e_token_type[]){WORD, REDIR_OUT, REDIR_IN},
			.expected_argv = (char *[]){NULL},
			.expected_filenames = (char *[]){NULL},
			.expected_fds = (int []){0},
			.expected_redir_types = (enum e_redir_type[]){0},
			.expected_return = 1,
			.expected_redirection_count = 0,
			.test_case_name = "Error case - double redirection > <"
		},
		// Test 10: Error case - double redirection << <
		{
			.tokens = (char *[]){"ls", "<<", "<", NULL},
			.token_types = (enum e_token_type[]){WORD, HERE_DOC, REDIR_IN},
			.expected_argv = (char *[]){NULL},
			.expected_filenames = (char *[]){NULL},
			.expected_fds = (int []){0},
			.expected_redir_types = (enum e_redir_type[]){0},
			.expected_return = 1,
			.expected_redirection_count = 0,
			.test_case_name = "Error case - double redirection << <"
		},
		// Test 11: Input only quotes
		{
			.tokens = (char *[]){"\"\"", NULL},
			.token_types = (enum e_token_type[]){WORD},
			.expected_argv = (char *[]){NULL},
			.expected_filenames = (char *[]){NULL},
			.expected_fds = (int []){0},
			.expected_redir_types = (enum e_redir_type[]){0},
			.expected_return = 0,
			.expected_redirection_count = 0,
			.test_case_name = "Input only quotes"
		}
	};

    int total_errors = 0;
	int errors = 0;
    int test_count = sizeof(tests) / sizeof(tests[0]);
    int i;
    
	printf("================= UNIT TESTS FOR PARSING =================\n");

    for (i = 0; i < test_count; i++)
    {
        printf("\n%d. ", i + 1);
        errors = run_single_command_test(&tests[i]);
		if (errors == 0)
			printf("\033[32mPASSED\033[0m\n");
		else
			printf("\033[31mFAILED\033[0m\n");
		total_errors = total_errors + errors;
    }
    
    printf("\n========================================\n");
    printf("Test Summary: %d tests run, %d errors found\n", test_count, total_errors);
    printf("========================================\n");
    
    return total_errors;
}
