/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_unit_testing.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 16:56:56 by bastalze          #+#    #+#             */
/*   Updated: 2026/07/24 13:58:07 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../minishell.h"
#include "../testing.h"

// ==================== TEST STRUCTURE ====================

typedef struct {
    char *input;                     // The command to test
    char *expected_tokens[10];       // Expected token strings
	enum e_token_type expected_types[10]; // Expected token types
    int expected_count;              // How many tokens expected
    int expect_error;                // 1 = expect error, 0 = expect success
    char *test_name;                 // Name of the test
} test_case;

// ==================== TEST RUNNER ====================

void run_test(test_case tc, t_single_linked_node *env) 
{
    t_token_node token_lst[100];
    t_token_iteri iteri;
    
    // Initialize
    ft_bzero(token_lst, sizeof(t_token_node) * 100);
    ft_bzero(&iteri, sizeof(t_token_iteri));
    
    printf("-> %s\n", tc.test_name);
    printf("   Input: '%s'\n", tc.input);
    
    // Run tokenization
    int result = tokenization(tc.input, env, token_lst, &iteri);
    
    // Check if we expected an error
    if (tc.expect_error) {
        if (result == 1) {
            printf("   ✅ Expected error occurred\n\n");
        } else {
            printf("   ❌ Expected error but got success\n\n");
        }
        return;
    }
    
    // Check for unexpected error
    if (result == 1) {
        printf("   ❌ Unexpected error\n\n");
        return;
    }
    
    // Check token count
	int i = 0;
	while(token_lst[i].token_type)
		i++;
    if (i != tc.expected_count) {
        printf("   ❌ Wrong token count\n");
        printf("      Expected: %d tokens\n", tc.expected_count);
        printf("      Got:      %d tokens\n", iteri.token);
        printf("\n");
        return;
    }
    
    // Check each token
    for (int i = 0; i < tc.expected_count; i++) {
        if (strcmp(token_lst[i].token_str, tc.expected_tokens[i]) != 0) {
            printf("   ❌ Token %d mismatch\n", i);
            printf("      Expected: '%s'\n", tc.expected_tokens[i]);
            printf("      Got:      '%s'\n", token_lst[i].token_str);
            printf("\n");
            return;
        }
        if (token_lst[i].token_type != tc.expected_types[i]) {
            printf("   ❌ Token %d type mismatch\n", i);
            printf("      Expected type: %d\n", tc.expected_types[i]);
            printf("      Got type:      %d\n", token_lst[i].token_type);
            printf("\n");
            return;
        }
    }
    
    printf("   ✅ All tokens match!\n\n");
}

// ==================== HELPER MACRO ====================

// Makes defining tokens easier
#define TOKEN(str, type) str, type

// ==================== TEST CASES ====================

void tokenization_unit_testing(t_single_linked_node *env) 
{
    test_case tests[] = {
        // ===== SIMPLE COMMANDS =====
        {
            .test_name = "Simple echo",
            .input = "echo hello world",
            .expected_tokens = {"echo", "hello", "world"},
            .expected_types = {WORD, WORD, WORD},
            .expected_count = 3,
            .expect_error = 0
        },
        {
            .test_name = "LS with flag",
            .input = "ls -la",
            .expected_tokens = {"ls", "-la"},
            .expected_types = {WORD, WORD},
            .expected_count = 2,
            .expect_error = 0
        },
        {
            .test_name = "Cat with file",
            .input = "cat file.txt",
            .expected_tokens = {"cat", "file.txt"},
            .expected_types = {WORD, WORD},
            .expected_count = 2,
            .expect_error = 0
        },
        
        // ===== REDIRECTIONS =====
        {
            .test_name = "Output redirection",
            .input = "cat > out.txt",
            .expected_tokens = {"cat", ">", "out.txt"},
            .expected_types = {WORD, REDIR_OUT, WORD},
            .expected_count = 3,
            .expect_error = 0
        },
        {
            .test_name = "Append redirection",
            .input = "cat >> out.txt",
            .expected_tokens = {"cat", ">>", "out.txt"},
            .expected_types = {WORD, REDIR_OUT_A, WORD},
            .expected_count = 3,
            .expect_error = 0
        },
        {
            .test_name = "Input redirection",
            .input = "cat < in.txt",
            .expected_tokens = {"cat", "<", "in.txt"},
            .expected_types = {WORD, REDIR_IN, WORD},
            .expected_count = 3,
            .expect_error = 0
        },
        
        // ===== PIPES =====
        {
            .test_name = "Single pipe",
            .input = "ls | grep .c",
            .expected_tokens = {"ls", "|", "grep", ".c"},
            .expected_types = {WORD, PIPE, WORD, WORD},
            .expected_count = 4,
            .expect_error = 0
        },
        {
            .test_name = "Multiple pipes",
            .input = "ls | grep .c | wc -l",
            .expected_tokens = {"ls", "|", "grep", ".c", "|", "wc", "-l"},
            .expected_types = {WORD, PIPE, WORD, WORD, PIPE, WORD, WORD},
            .expected_count = 7,
            .expect_error = 0
        },
        
        // ===== QUOTES =====
        {
            .test_name = "Double quotes",
            .input = "echo \"hello world\"",
            .expected_tokens = {"echo", "\"hello world\""},
            .expected_types = {WORD, WORD},
            .expected_count = 2,
            .expect_error = 0
        },
        {
            .test_name = "Single quotes",
            .input = "echo 'test'",
            .expected_tokens = {"echo", "'test'"},
            .expected_types = {WORD, WORD},
            .expected_count = 2,
            .expect_error = 0
        },
        {
            .test_name = "Mixed quotes",
            .input = "echo \"hello 'world'\"",
            .expected_tokens = {"echo", "\"hello 'world'\""},
            .expected_types = {WORD, WORD},
            .expected_count = 2,
            .expect_error = 0
        },
        
        // ===== ERROR CASES =====
        {
            .test_name = "Unclosed double quote",
            .input = "echo \"unclosed",
            .expected_tokens = {},
            .expected_types = {},
            .expected_count = 0,
            .expect_error = 1
        },
        {
            .test_name = "Unclosed single quote",
            .input = "echo 'unclosed",
            .expected_tokens = {},
            .expected_types = {},
            .expected_count = 0,
            .expect_error = 1
        },
        
        // ===== EDGE CASES =====
        {
            .test_name = "Empty input",
            .input = "",
            .expected_tokens = {},
            .expected_types = {},
            .expected_count = 0,
            .expect_error = 0
        },
        {
            .test_name = "Only spaces",
            .input = "   ",
            .expected_tokens = {},
            .expected_types = {},
            .expected_count = 0,
            .expect_error = 0
        },
        {
            .test_name = "Trailing spaces",
            .input = "echo  ",
            .expected_tokens = {"echo"},
            .expected_types = {WORD},
            .expected_count = 1,
            .expect_error = 0
        },
        {
            .test_name = "Leading spaces",
            .input = "   echo",
            .expected_tokens = {"echo"},
            .expected_types = {WORD},
            .expected_count = 1,
            .expect_error = 0
        },
        {
            .test_name = "Multiple spaces between tokens",
            .input = "echo    hello",
            .expected_tokens = {"echo", "hello"},
            .expected_types = {WORD, WORD},
            .expected_count = 2,
            .expect_error = 0
        },
        
        // ===== COMPLEX CASES =====
        {
            .test_name = "Pipeline with redirections",
            .input = "cat < in.txt | grep hello > out.txt",
            .expected_tokens = {"cat", "<", "in.txt", "|", "grep", "hello", ">", "out.txt"},
            .expected_types = {WORD, REDIR_IN, WORD, PIPE, WORD, WORD, REDIR_OUT, WORD},
            .expected_count = 8,
            .expect_error = 0
        },
        {
            .test_name = "Multiple redirections",
            .input = "cmd < in.txt > out.txt 2> error.txt",
            .expected_tokens = {"cmd", "<", "in.txt", ">", "out.txt", "2>", "error.txt"},
            .expected_types = {WORD, REDIR_IN, WORD, REDIR_OUT, WORD, REDIR_OUT, WORD},
            .expected_count = 7,
            .expect_error = 0
        },

		// ==== UGLY CASES ====
        {
            .test_name = "Multiple redirections with weird spacing",
            .input = "cmd< in.txt>out.txt 2> error.txt",
            .expected_tokens = {"cmd", "<", "in.txt", ">", "out.txt", "2>", "error.txt"},
            .expected_types = {WORD, REDIR_IN, WORD, REDIR_OUT, WORD, REDIR_OUT, WORD},
            .expected_count = 7,
            .expect_error = 0
        },
        {
            .test_name = "Multiple redirections with weird spacing",
            .input = "cmd< in.txt>out.txt >error.txt",
            .expected_tokens = {"cmd", "<", "in.txt", ">", "out.txt", ">", "error.txt"},
            .expected_types = {WORD, REDIR_IN, WORD, REDIR_OUT, WORD, REDIR_OUT, WORD},
            .expected_count = 7,
            .expect_error = 0
        },
        {
            .test_name = "Multiple pipes with weird spacing",
            .input = "cat file.txt|<ubi.c ls|echo",
            .expected_tokens = {"cat", "file.txt", "|", "<", "ubi.c", "ls", "|", "echo"},
            .expected_types = {WORD, WORD, PIPE, REDIR_IN, WORD, WORD, PIPE, WORD},
            .expected_count = 8,
            .expect_error = 0
        }

    };
    
    int num_tests = sizeof(tests) / sizeof(test_case);
    printf("╔═══════════════════════════════════════╗\n");
    printf("║   Running %d test cases               ║\n", num_tests);
    printf("╚═══════════════════════════════════════╝\n\n");
    
    for (int i = 0; i < num_tests; i++) {
        run_test(tests[i], env);
    }
}
