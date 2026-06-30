/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 19:30:18 by nilsdruon         #+#    #+#             */
/*   Updated: 2026/06/29 19:51:43 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include "testing.h"

typedef struct s_export_test
{
    char    *test_name;
    char    **input;
    int     expected_status;
    char    *expected_stderr;
    int     setup_type;          // 0: default, 1: single node list, 2: empty list
    char    *check_added_key;    // Key to verify exists after export
    char    *check_added_value;  // Value to verify match (NULL if key-only / no value)
}   t_export_test;

static t_single_linked_node *create_mock_env(int setup_type, char **original_envp)
{
    if (setup_type == 2)
        return (NULL);
    if (setup_type == 1)
    {
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

static int verify_key_value(t_single_linked_node *env_lst, const char *key, const char *expected_val)
{
    if (!key)
        return (1);
    while (env_lst)
    {
        t_env_var *var = (t_env_var *)env_lst->content;
        if (var && var->key && strcmp(var->key, key) == 0)
        {
            if (!expected_val && !var->value)
                return (1);
            if (expected_val && var->value && strcmp(var->value, expected_val) == 0)
                return (1);
            return (0); // Key matches but value does not
        }
        env_lst = env_lst->next;
    }
    return (0); // Key wasn't found at all
}

static int run_single_export_test(t_export_test test, char **original_envp)
{
    int                     saved_stdout;
    int                     saved_stderr;
    int                     out_pipe[2];
    int                     err_pipe[2];
    char                    out_buffer[16384];
    char                    err_buffer[4096];
    int                     actual_status;
    int                     pass = 1;
    t_single_linked_node    *env_lst;

    env_lst = create_mock_env(test.setup_type, original_envp);

    if (pipe(out_pipe) == -1 || pipe(err_pipe) == -1)
    {
        perror("pipe failed");
        if (env_lst)
            ft_single_lstclear(&env_lst, del_env_node_content);
        return (0);
    }

    saved_stdout = dup(STDOUT_FILENO);
    saved_stderr = dup(STDERR_FILENO);
    
    dup2(out_pipe[1], STDOUT_FILENO);
    close(out_pipe[1]);
    dup2(err_pipe[1], STDERR_FILENO);
    close(err_pipe[1]);

    // CALLING YOUR EXPORT IMPLEMENTATION
    actual_status = export(test.input, env_lst);

    fflush(stdout);
    fflush(stderr);
    dup2(saved_stdout, STDOUT_FILENO);
    dup2(saved_stderr, STDERR_FILENO);
    close(saved_stdout);
    close(saved_stderr);

    memset(out_buffer, 0, sizeof(out_buffer));
    fcntl(out_pipe[0], F_SETFL, O_NONBLOCK);
    read(out_pipe[0], out_buffer, sizeof(out_buffer) - 1);
    close(out_pipe[0]);

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

    // 2. Verify Output text (when calling export with no arguments)
    if (!test.input[1] && actual_status == 0)
    {
        if (out_buffer[0] == '\0')
        {
            if (pass) printf("❌ FAIL: %s [STDOUT Empty]\n", test.test_name);
            printf("   Expected declare -x prints but got nothing.\n");
            pass = 0;
        }
    }

    // 3. Verify Error Messages (Stderr)
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

    // 4. Structural State Integrity Verification
    if (pass && test.check_added_key)
    {
        if (!verify_key_value(env_lst, test.check_added_key, test.check_added_value))
        {
            printf("❌ FAIL: %s [State Corruption / Missing Node]\n", test.test_name);
            printf("   Expected environment to contain %s=%s\n", 
                test.check_added_key, test.check_added_value ? test.check_added_value : "(null)");
            pass = 0;
        }
    }

    if (pass)
        printf("✅ PASS: %s\n", test.test_name);

    if (env_lst)
        ft_single_lstclear(&env_lst, del_env_node_content);
    return (pass);
}

int test_export(char **envp)
{
    int success = 1;

    // Test Cases Input definitions
    char *case1[] = {"export", NULL};
    char *case2[] = {"export", "NEW_VAR=hello", NULL};
    char *case3[] = {"export", "KEY_NO_VALUE", NULL};
    char *case4[] = {"export", "LONELY_VAR=mutated_value", NULL};
    char *case5[] = {"export", "EMPTY_VAL=", NULL};
    char *case6[] = {"export", "VAR_A=1", "VAR_B=2", "VAR_C=3", NULL};
    char *case7[] = {"export", "invalid-id=123", NULL};
    char *case8[] = {"export", "123var=true", NULL};
    char *case9[] = {"export", "=value", NULL};
    char *case10[] = {"export", "", NULL};
    char *case11[] = {"export", "VALID_ONE=ok", "bad@id=no", "VALID_TWO=yes", NULL};

    t_export_test tests[] = {
        {"No arguments verification (Prints sorted env list)", case1, 0, NULL, 0, NULL, NULL},
        {"Add a brand new valid key-value pair", case2, 0, NULL, 0, "NEW_VAR", "hello"},
        {"Add a key without a value assignment", case3, 0, NULL, 0, "KEY_NO_VALUE", NULL},
        {"Modify value of an already existing key", case4, 0, NULL, 1, "LONELY_VAR", "mutated_value"},
        {"Add a key with an explicit empty value string", case5, 0, NULL, 0, "EMPTY_VAL", ""},
        {"Multiple valid variables added simultaneously", case6, 0, NULL, 0, "VAR_B", "2"},
        {"Edge Case: Export key-value into an empty list structure", case2, 0, NULL, 2, "NEW_VAR", "hello"},
        {"Invalid Identifier: key contains a hyphen", case7, 1, "not a valid identifier", 0, NULL, NULL},
        {"Invalid Identifier: key begins with numerical digits", case8, 1, "not a valid identifier", 0, NULL, NULL},
        {"Invalid Identifier: starts directly with assignment operator", case9, 1, "not a valid identifier", 0, NULL, NULL},
        {"Edge Case: Empty string passed as an identifier", case10, 1, "not a valid identifier", 0, NULL, NULL},
        {"Mixed validation: multi-args processing up to errors", case11, 1, "not a valid identifier", 0, "VALID_ONE", "ok"}
    };

    int num_tests = sizeof(tests) / sizeof(tests[0]);

    printf("\n--- Running Minishell Export Builtin Tests ---\n");
    for (int i = 0; i < num_tests; i++)
    {
        if (!run_single_export_test(tests[i], envp))
            success = 0;
    }

    return (success);
}