/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 16:29:58 by nildruon          #+#    #+#             */
/*   Updated: 2026/06/30 00:24:17 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "testing.h"

typedef struct s_unset_test
{
    char    *test_name;
    char    **input;
    int     expected_status;
    char    *expected_stderr;
    int     setup_type;         // 0: default, 1: single node list, 2: empty list
    char    *check_missing_key; // Key to verify was successfully removed
}   t_unset_test;

static t_single_linked_node *create_mock_env(int setup_type, char **original_envp)
{
    if (setup_type == 2)
        return (NULL);
    if (setup_type == 1)
    {
        // Construct a list with exactly one controlled node
        t_single_linked_node *node = malloc(sizeof(t_single_linked_node));
        t_env_var            *var = malloc(sizeof(t_env_var));
        if (!node || !var)
            return (NULL);
        var->key = strdup("LONELY_VAR");
        var->value = strdup("alone_in_the_dark");
        node->content = var;
        node->next = NULL;
        return (node);
    }
    return (env_to_lst(original_envp));
}

static int verify_key_missing(t_single_linked_node *env_lst, const char *key)
{
    if (!key)
        return (1);
    while (env_lst)
    {
        t_env_var *var = (t_env_var *)env_lst->content;
        if (var && var->key && strcmp(var->key, key) == 0)
            return (0); // Found it! That's a failure if we expected it gone.
        env_lst = env_lst->next;
    }
    return (1);
}

static int run_single_unset_test(t_unset_test test, char **original_envp)
{
    int                     saved_stderr;
    int                     err_pipe[2];
    char                    err_buffer[4096];
    int                     actual_status;
    int                     pass = 1;
    t_single_linked_node    *env_lst;

    env_lst = create_mock_env(test.setup_type, original_envp);

    if (pipe(err_pipe) == -1)
    {
        perror("pipe failed");
        if (env_lst)
            ft_single_lstclear(&env_lst, del_env_node_content);
        return (0);
    }

    saved_stderr = dup(STDERR_FILENO);
    dup2(err_pipe[1], STDERR_FILENO);
    close(err_pipe[1]);

    // CALLING YOUR UNSET IMPLEMENTATION
    actual_status = unset(test.input, &env_lst);

    fflush(stderr);
    dup2(saved_stderr, STDERR_FILENO);
    close(saved_stderr);

    memset(err_buffer, 0, sizeof(err_buffer));
    fcntl(err_pipe[0], F_SETFL, O_NONBLOCK);
    read(err_pipe[0], err_buffer, sizeof(err_buffer) - 1);
    close(err_pipe[0]);

    // 1. Verify Exit Code
    if (actual_status != test.expected_status)
    {
        printf("❌ FAIL: %s [Exit Status Mismatch]\n", test.test_name);
        printf("   Expected status: %d, Got: %d\n", test.expected_status, actual_status);
        pass = 0;
    }

    // 2. Verify Error Messages (Stderr)
    if (test.expected_stderr)
    {
        if (strstr(err_buffer, test.expected_stderr) == NULL)
        {
            if (pass) printf("❌ FAIL: %s [STDERR Mismatch]\n", test.test_name);
            printf("   Expected Error Msg to contain: \"%s\"\n", test.expected_stderr);
            printf("   Got Error Msg                : \"%s\"\n", err_buffer);
            pass = 0;
        }
    }
    else if (err_buffer[0] != '\0')
    {
        if (pass) printf("❌ FAIL: %s [Unexpected Error Output]\n", test.test_name);
        printf("   Got unexpected stderr noise: \"%s\"\n", err_buffer);
        pass = 0;
    }

    // 3. Structural State Integrity Verification
    if (pass && test.check_missing_key)
    {
        if (!verify_key_missing(env_lst, test.check_missing_key))
        {
            printf("❌ FAIL: %s [State Corruption]\n", test.test_name);
            printf("   Variable '%s' was expected to be unlinked and freed, but still exists.\n", test.check_missing_key);
            pass = 0;
        }
    }

    if (pass)
        printf("✅ PASS: %s\n", test.test_name);

    if (env_lst)
        ft_single_lstclear(&env_lst, del_env_node_content);
    return (pass);
}

int test_unset(char **envp)
{
    int success = 1;

    // Head, Mid, Tail targets depend on normal env variables standardly found (like USER, PATH, HOME)
    char *case1[] = {"unset", NULL};
    char *case2[] = {"unset", "NON_EXISTENT_VARIABLE_XYZ", NULL};
    char *case3[] = {"unset", "USER", NULL};
    char *case4[] = {"unset", "PATH", NULL};
    char *case5[] = {"unset", "LONELY_VAR", NULL};
    char *case6[] = {"unset", "USER", "PATH", "HOME", NULL};
    char *case7[] = {"unset", "invalid-id", NULL};
    char *case8[] = {"unset", "123variable", NULL};
    char *case9[] = {"unset", "", NULL};
    char *case10[] = {"unset", "USER", "invalid@id", "PATH", NULL}; // mixed scenario

    // Status updated to 0 and error messages cleared to match standard flagless logic
    t_unset_test tests[] = {
        {"No arguments verification (No-op)", case1, 0, NULL, 0, NULL},
        {"Target variable does not exist (No-op success)", case2, 0, NULL, 0, NULL},
        {"Unset normal existing environment variable", case3, 0, NULL, 0, "USER"},
        {"Unset another target variable sequential validation", case4, 0, NULL, 0, "PATH"},
        {"Edge Case: Unset the absolute only node in the list", case5, 0, NULL, 1, "LONELY_VAR"},
        {"Edge Case: Unset on a completely empty environment list", case3, 0, NULL, 2, NULL},
        {"Multiple arguments processed simultaneously", case6, 0, NULL, 0, "HOME"},
        {"Invalid Identifier: contains a hyphen", case7, 0, NULL, 0, NULL},
        {"Invalid Identifier: starts with numeric digits", case8, 0, NULL, 0, NULL},
        {"Edge Case: Empty string passed as an identifier", case9, 0, NULL, 0, NULL},
        {"Mixed validation: multi-args with one invalid option", case10, 0, NULL, 0, "USER"}
    };

    int num_tests = sizeof(tests) / sizeof(tests[0]);

    printf("\n--- Running Minishell Unset Builtin Tests ---\n");
    for (int i = 0; i < num_tests; i++)
    {
        if (!run_single_unset_test(tests[i], envp))
            success = 0;
    }

    return (success);
}