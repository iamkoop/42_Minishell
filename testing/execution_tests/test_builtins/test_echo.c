/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 18:02:46 by nildruon          #+#    #+#             */
/*   Updated: 2026/05/27 18:24:14 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "testing.h"

#include "testing.h"

typedef struct s_echo_test
{
    char    *test_name;
    char    **input;
    char    *expected_output;
}   t_echo_test;

int test_echo(void)
{
    int pipefd[2];
    int success = 1;

    // Fixed arrays on the stack — No allocations, perfectly leak-free
    char *case1[] = {"echo", NULL};
    char *case2[] = {"echo", "-n", NULL};
    char *case3[] = {"echo", "hello", "world", NULL};
    char *case4[] = {"echo", "-n", "hello", "world", NULL};
    char *case5[] = {"echo", "-nnnnn", "-n", "multiple", "n", NULL};
    char *case6[] = {"echo", "-nx", "invalid", "flag", NULL};
    char *case7[] = {"echo", "strings", "", "empty", NULL};
    char *case8[] = {"echo", "-n", "", NULL};

    t_echo_test tests[] = {
        {"Plain echo (Newline only)", case1, "\n"},
        {"Echo single -n flag", case2, ""},
        {"Standard multiple arguments", case3, "hello world\n"},
        {"Arguments with -n flag", case4, "hello world"},
        {"Consecutive/stacked valid flags", case5, "multiple n"},
        {"Invalid flag interpretation (-nx treated as string)", case6, "-nx invalid flag\n"},
        {"Empty string middle argument", case7, "strings  empty\n"},
        {"Empty string with -n flag", case8, ""},
    };

    int num_tests = sizeof(tests) / sizeof(tests[0]);

    printf("\n--- Running Echo Builtin Tests ---\n");

    for (int i = 0; i < num_tests; i++)
    {
        if (pipe(pipefd) == -1)
        {
            perror("pipe failed");
            return (0);
        }

        int saved_stdout = dup(STDOUT_FILENO);
        if (saved_stdout == -1)
        {
            perror("dup failed");
            close(pipefd[0]);
            close(pipefd[1]);
            return (0);
        }

        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        // ==========================================
        // CALLING YOUR ECHO IMPLEMENTATION
        // ==========================================
        echo(tests[i].input);
        // ==========================================

        fflush(stdout);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);

        // Fixed-size buffer stack allocation
        char buffer[4096];
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1);
        close(pipefd[0]);

        if (bytes_read < 0)
        {
            perror("read failed");
            return (0);
        }

        if (strcmp(buffer, tests[i].expected_output) == 0)
        {
            printf("✅ PASS: %s\n", tests[i].test_name);
        }
        else
        {
            printf("❌ FAIL: %s\n", tests[i].test_name);
            printf("   Expected: \"%s\"\n", tests[i].expected_output);
            printf("   Got     : \"%s\"\n", buffer);
            success = 0;
        }
    }

    return (success);
}