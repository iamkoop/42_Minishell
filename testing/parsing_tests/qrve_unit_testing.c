/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   qrve_unit_testing.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 20:49:46 by bastalze          #+#    #+#             */
/*   Updated: 2026/07/28 11:06:26 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../minishell.h"
#include "../testing.h"

typedef struct {
    char *input;
    char expected_words[WORD_AMOUNT][WORD_STR_SIZE];
    int expected_return;
    bool expect_error;
    char *description;
	bool heredoc;
} t_test_case;

void print_word_array(char words[WORD_AMOUNT][WORD_STR_SIZE], int count) {
    printf("[");
    for (int i = 0; i < count && words[i][0] != '\0'; i++) {
        if (i > 0) printf(", ");
        printf("\"%s\"", words[i]);
    }
    printf("]");
}

int count_words(char words[WORD_AMOUNT][WORD_STR_SIZE]) {
    int count = 0;
    while (count < WORD_AMOUNT && words[count][0] != '\0') {
        count++;
    }
    return count;
}

bool compare_word_arrays(char actual[WORD_AMOUNT][WORD_STR_SIZE],
                         char expected[WORD_AMOUNT][WORD_STR_SIZE]) {
    int actual_count = count_words(actual);
    int expected_count = count_words(expected);

    if (actual_count != expected_count) {
        return false;
    }

    for (int i = 0; i < actual_count; i++) {
        if (strncmp(actual[i], expected[i], WORD_STR_SIZE - 1) != 0) {
            return false;
        }
    }
    return true;
}

void print_word_mismatch(char actual[WORD_AMOUNT][WORD_STR_SIZE],
                         char expected[WORD_AMOUNT][WORD_STR_SIZE]) {
    int actual_count = count_words(actual);
    int expected_count = count_words(expected);

    printf("\n  Expected words: ");
    print_word_array(expected, expected_count);
    printf("\n  Actual words:   ");
    print_word_array(actual, actual_count);
    printf("\n");

    // Find first mismatch
	int max_count;
	if (actual_count > expected_count)
		max_count = actual_count;
	else
		max_count = expected_count;
    for (int i = 0; i < max_count; i++) {
        char expected_str[WORD_STR_SIZE] = "(missing)";
        char actual_str[WORD_STR_SIZE] = "(missing)";

        if (i < expected_count) strcpy(expected_str, expected[i]);
        if (i < actual_count) strcpy(actual_str, actual[i]);

        if (strcmp(expected_str, actual_str) != 0) {
            printf("  First mismatch at word[%d]: expected \"%s\", got \"%s\"\n",
                   i, expected_str, actual_str);
            break;
        }
    }
}

// ==================== TEST EXECUTION ====================

void    qrve_unit_testing(t_single_linked_node *env)
{
 	int total = 0;
    int passed = 0;
    int failed = 0;

    // Test cases array
    t_test_case tests[] = {
        // Test 1: Simple variable expansion
        {
            .input = "Hello$VAR",
            .expected_words = {"HelloHamster", "cage"},
            .expected_return = 0,
            .expect_error = false,
            .description = "Simple variable expansion",
			.heredoc = false
        },
        // Test 2: Multiple variables
        {
            .input = "$VAR$VAR",
            .expected_words = {"Hamster", "cageHamster", "cage"},
            .expected_return = 0,
            .expect_error = false,
            .description = "Multiple variable expansion",
			.heredoc = false
        },
        // Test 3: Quoted variable
        {
            .input = "\"$VAR\"",
            .expected_words = {"Hamster cage"},
            .expected_return = 0,
            .expect_error = false,
            .description = "Quoted variable expansion",
			.heredoc = false
        },
        // Test 4: Error case - invalid variable
        {
            .input = "$INVALID_VAR",
            .expected_words = {""},
            .expected_return = 0,
            .expect_error = false,
            .description = "Invalid variable should be replaced with empty string",
			.heredoc = false
        },
        // Test 4.1: Dollar with no name after
        {
            .input = "$...",
            .expected_words = {"$..."},
            .expected_return = 0,
            .expect_error = false,
            .description = "Dollar with no name after",
			.heredoc = false
        },
        // Test 4.2: Dollar with no name after 2
        {
            .input = "$9VAR",
            .expected_words = {"$9VAR"},
            .expected_return = 0,
            .expect_error = false,
            .description = "Dollar with no name after 2",
			.heredoc = false
        },
        // Test 4.2: Only dollar
        {
            .input = "$",
            .expected_words = {"$"},
            .expected_return = 0,
            .expect_error = false,
            .description = "Only dollar",
			.heredoc = false
        },
        // Test 5: Empty input
        {
            .input = "",
            .expected_words = {""},
            .expected_return = 0,
            .expect_error = false,
            .description = "Empty input string",
			.heredoc = false
        },
        // Test 5.1: Single quotes
        {
            .input = "\'$VAR\'",
            .expected_words = {"$VAR"},
            .expected_return = 0,
            .expect_error = false,
            .description = "Single quotes",
			.heredoc = false
        },
        // Test 5.2: Quotes within quotes
        {
            .input = "\'\"$VAR\"\'",
            .expected_words = {"\"$VAR\""},
            .expected_return = 0,
            .expect_error = false,
            .description = "Double quotes within single quotes",
			.heredoc = false
        },
        // Test 5.3: Quotes within quotes plus text
        {
            .input = "\'Here we go \"$VAR\"\'",
            .expected_words = {"Here we go \"$VAR\""},
            .expected_return = 0,
            .expect_error = false,
            .description = "Double quotes within single quotes + extra text",
			.heredoc = false
        },
        // Test 6: Heredoc mode
        {
            .input = "$VAR",
            .expected_words = {"Hamster cage"},
            .expected_return = 0,
            .expect_error = false,
            .description = "Variable expansion in heredoc mode",
			.heredoc = true
        },
        // Test 7: Quoted heredoc mode
        {
            .input = "\"$VAR\"",
            .expected_words = {"\"Hamster cage\""},
            .expected_return = 0,
            .expect_error = false,
            .description = "Quoted in heredoc mode",
			.heredoc = true
        },
        // Test 8: Error word too long
        {
            .input = "12345678901234567890123456789012345678901234567890",
            .expected_words = {},
            .expected_return = 1,
            .expect_error = true,
            .description = "Error word too long",
			.heredoc = true
        }
    };

    int num_tests = sizeof(tests) / sizeof(t_test_case);
    
    printf("\n========== TESTS FOR QUOTE REMOVAL AND VARIABLE EXPANSION ==========\n\n");

	    for (int i = 0; i < num_tests; i++) {
        char actual_words[WORD_AMOUNT][WORD_STR_SIZE] = {0};
        t_test_case *test = &tests[i];

        printf("Test \"%s\": ", test->description);
        total++;

	t_quote_iteri   exv;
	ft_bzero(&exv, sizeof(t_quote_iteri));
	exv.heredoc = test->heredoc;

        // Call function
        int return_value = quote_rm_var_expan(test->input, actual_words, env, &exv);

        bool error_expected = test->expect_error;
        bool got_error = (return_value != 0);
        bool test_passed = true;

        // 1. Error-Check
        if (error_expected && !got_error) {
            printf("\033[31mFAILED\033[0m - expected error but got success\n");
            test_passed = false;
        } else if (!error_expected && got_error) {
            printf("\033[31mFAILED\033[0m - unexpected error (return value: %d)\n", return_value);
            test_passed = false;
        }
    	// 2. Word-Array Check (when no error)
        else if (!error_expected && !compare_word_arrays(actual_words, test->expected_words)) {
            printf("\033[31mFAILED\033[0m - word array mismatch\n");
            print_word_mismatch(actual_words, test->expected_words);
            test_passed = false;
        }

        // 3. Print results
        if (test_passed) {
            printf("\033[32mPASSED\033[0m");
            if (error_expected) {
                printf(" (error correctly triggered)");
            }
            printf("\n");
            passed++;
        } else {
            printf("  Input: \"%s\"\n", test->input);
            failed++;
        }

        printf("\n");
    }

    // ===== SUMMARY =====
    printf("========== TEST SUMMARY ==========\n");
    printf("Total tests: %d\n", total);
    printf("\033[32mPassed: %d\033[0m\n", passed);
    if (failed > 0) {
        printf("\033[31mFailed: %d\033[0m\n", failed);
    } else {
        printf("Failed: 0\n");
    }
    printf("===================================\n\n");
}

