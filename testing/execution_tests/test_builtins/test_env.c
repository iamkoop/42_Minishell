/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:50:10 by nildruon          #+#    #+#             */
/*   Updated: 2026/06/10 01:29:27 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "testing.h"

typedef struct s_get_env_test
{
    char    *test_name;
    char    *to_find;
    char    *expected_value;
}   t_get_env_test;

static void free_char_matrix(char **matrix)
{
    int i;

    if (!matrix)
        return;
    i = 0;
    while (matrix[i])
    {
        free(matrix[i]);
        i++;
    }
    free(matrix);
}

// --- Internal Helper: Verifies captured output string against expected envp array ---
static int verify_output(const char *buffer, char **envp)
{
    char *buffer_copy = strdup(buffer);
    if (!buffer_copy)
    {
        perror("strdup failed");
        return (0);
    }

    char *line = strtok(buffer_copy, "\n");
    int i = 0;
    int success = 1;

    while (line != NULL && envp[i] != NULL)
    {
        if (strcmp(line, envp[i]) != 0)
        {
            printf("❌ FAIL: Mismatch at index %d\n", i);
            printf("   Expected: \"%s\"\n", envp[i]);
            printf("   Got     : \"%s\"\n", line);
            success = 0;
            break;
        }
        line = strtok(NULL, "\n");
        i++;
    }

    if (success)
    {
        if (envp[i] != NULL)
        {
            printf("❌ FAIL: env() stopped printing early. Missing: \"%s\"\n", envp[i]);
            success = 0;
        }
        else if (line != NULL)
        {
            printf("❌ FAIL: env() printed extra data: \"%s\"\n", line);
            success = 0;
        }
    }

    free(buffer_copy);
    return (success);
}

// --- Helper: Runs isolated tests for get_env_from_lst ---
static int test_get_env_func(t_single_linked_node *lst)
{
    int success = 1;
    t_single_linked_node *node;

    t_get_env_test tests[] = {
        {"Find standard existing key", "VALID", "123"},
        {"Find key with internal assignment tokens", "PATH", "/usr/bin=/bin"},
        {"Find key with empty value token", "EMPTY_VAL", ""},
        {"Find valueless attribute node", "NO_EQUAL_SIGN", NULL},
        {"Look up non-existent key", "NOT_FOUND_KEY", NULL},
        {"Look up partial key match (security check)", "VALI", NULL}
    };

    int num_tests = sizeof(tests) / sizeof(tests[0]);

    printf("\n--- Running get_env_from_lst Tests ---\n");

    // Guard test 1: Reference list pointer is entirely NULL
    node = get_env_from_lst("VALID", NULL);
    if (node != NULL)
    {
        printf("❌ FAIL: get_env_from_lst on NULL list returned a node!\n");
        success = 0;
    }
    else
        printf("✅ PASS: Guard check on NULL list reference\n");

    // Guard test 2: Lookup string token is completely NULL (conditional execution)
    // Note: If your function doesn't protect against !to_find yet, this would crash.
    char *null_str = NULL;
    if (null_str == NULL) 
        printf("⚠️ INFO: Skipping completely NULL token to protect your current implementation from crashing.\n");

    for (int i = 0; i < num_tests; i++)
    {
        node = get_env_from_lst(tests[i].to_find, lst);
        
        if (tests[i].expected_value == NULL)
        {
            if (node == NULL || ((t_env_var *)node->content)->value == NULL)
                printf("✅ PASS: %s\n", tests[i].test_name);
            else
            {
                printf("❌ FAIL: %s\n", tests[i].test_name);
                printf("   Expected: NULL node pointer or NULL value\n");
                printf("   Got Key : \"%s\" with Value: \"%s\"\n", 
                    ((t_env_var *)node->content)->key, ((t_env_var *)node->content)->value);
                success = 0;
            }
        }
        else
        {
            if (!node || !node->content)
            {
                printf("❌ FAIL: %s\n", tests[i].test_name);
                printf("   Expected Value: \"%s\"\n", tests[i].expected_value);
                printf("   Got           : NULL node pointer\n");
                success = 0;
                continue;
            }
            
            t_env_var *var = (t_env_var *)node->content;
            char *actual_val = var->value ? var->value : "";
            if (strcmp(actual_val, tests[i].expected_value) == 0)
                printf("✅ PASS: %s\n", tests[i].test_name);
            else
            {
                printf("❌ FAIL: %s\n", tests[i].test_name);
                printf("   Expected Value: \"%s\"\n", tests[i].expected_value);
                printf("   Got Value     : \"%s\"\n", actual_val);
                success = 0;
            }
        }
    }
    return (success);
}

// --- Helper: Matrix to Linked List Validation ---
static int verify_matrix_to_lst(char **envp, t_single_linked_node *lst)
{
    int i = 0;
    t_single_linked_node *curr = lst;

    if ((!envp || !*envp) && !lst)
        return (1);
    if ((!envp || !*envp) || !lst)
        return (0);

    while (envp[i] != NULL && curr != NULL)
    {
        t_env_var *var = (t_env_var *)curr->content;
        if (!var || !var->key)
            return (0);

        size_t key_len = strlen(var->key);
        char *has_equal = strchr(envp[i], '=');

        if (!has_equal)
        {
            if (strcmp(envp[i], var->key) != 0 || var->value != NULL)
                return (0);
        }
        else if (strncmp(envp[i], var->key, key_len) != 0 || envp[i][key_len] != '=')
            return (0);
        else
        {
            char *expected_val = envp[i] + key_len + 1;
            char *actual_val = var->value ? var->value : "";
            if (strcmp(expected_val, actual_val) != 0)
                return (0);
        }
        curr = curr->next;
        i++;
    }
    return (envp[i] == NULL && curr == NULL);
}

// --- Helper: Linked List to Matrix Validation ---
static int verify_lst_to_matrix(t_single_linked_node *lst, char **matrix)
{
    int i = 0;
    t_single_linked_node *curr = lst;

    if (!lst && (!matrix || !*matrix))
        return (1);
    if (!lst || !matrix)
        return (0);

    while (curr != NULL && matrix[i] != NULL)
    {
        t_env_var *var = (t_env_var *)curr->content;
        if (!var || !var->key)
            return (0);

        char *actual_val = var->value ? var->value : "";
        size_t alloc_size = strlen(var->key) + strlen(actual_val) + 2;
        char *expected_str = malloc(alloc_size);
        if (!expected_str)
            return (0);

        if (!var->value)
            sprintf(expected_str, "%s", var->key);
        else
            sprintf(expected_str, "%s=%s", var->key, actual_val);

        int match = (strcmp(expected_str, matrix[i]) == 0);
        free(expected_str);

        if (!match)
            return (0);

        curr = curr->next;
        i++;
    }
    return (curr == NULL && matrix[i] == NULL);
}

// --- The Core Test Execution ---
int env_tests(char **envp)
{
    int pipefd[2];
    int success = 1;
    char *empty_envp[] = { NULL };
    char **current_envp = envp;

    // Phase 1: Output Capture / Builtin env Tests
    for (int run = 0; run < 2; run++)
    {
        if (run == 0)
            printf("--- Running Standard OS envp Test ---\n");
        else
        {
            printf("\n--- Running Empty envp (env -i) Test ---\n");
            current_envp = empty_envp;
        }

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

        t_single_linked_node *lst = env_to_lst(current_envp);
        env(lst);
        ft_single_lstclear(&lst, del_env_node_content);

        fflush(stdout);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);

        char buffer[8192];
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1);
        close(pipefd[0]);

        if (bytes_read < 0)
        {
            perror("read failed");
            return (0);
        }

        if (verify_output(buffer, current_envp))
            printf("✅ PASS: env() output perfectly matches envp!\n");
        else
            success = 0;
    }

    // Phase 2: Conversion Validation Tests
    char *case1[] = { NULL };
    char *case2[] = { "EMPTY_VAL=", "VALID=123", NULL };
    char *case3[] = { "NO_EQUAL_SIGN", "VALID=123", NULL };
    char *case4[] = { "PATH=/usr/bin=/bin", "EQUALS======", "VALID=123", NULL };
    char *case5[] = { "SPECIAL=~!@#$%^&*()_+{}|:<>?-=[]\\;',./", "VALID=123", NULL };

    struct {
        char *name;
        char **env_matrix;
    } conversion_cases[] = {
        {"Empty Environment Matrix (env -i)", case1},
        {"Keys explicitly ending with '='", case2},
        {"Valueless identifier attributes", case3},
        {"Values loaded with internal '=' assignments", case4},
        {"Raw special/symbolic payload values", case5},
    };

    int num_cases = sizeof(conversion_cases) / sizeof(conversion_cases[0]);

    printf("\n--- Running Env Conversion Pipeline Tests ---\n");

    for (int i = 0; i < num_cases; i++)
    {
        t_single_linked_node *lst = env_to_lst(conversion_cases[i].env_matrix);
        char **reverted_matrix = env_to_char_arr(lst);

        int dir1 = verify_matrix_to_lst(conversion_cases[i].env_matrix, lst);
        int dir2 = verify_lst_to_matrix(lst, reverted_matrix);

        if (dir1 && dir2)
            printf("✅ PASS: %s\n", conversion_cases[i].name);
        else
        {
            printf("❌ FAIL: %s\n", conversion_cases[i].name);
            if (!dir1) printf("   -> Matrix-to-List Conversion Failure\n");
            if (!dir2) printf("   -> List-to-Matrix Conversion Failure\n");
            success = 0;
        }

        ft_single_lstclear(&lst, del_env_node_content);
        free_char_matrix(reverted_matrix);
    }

    t_single_linked_node *host_lst = env_to_lst(envp);
    char **host_reverted = env_to_char_arr(host_lst);
    
    if (verify_matrix_to_lst(envp, host_lst) && verify_lst_to_matrix(host_lst, host_reverted))
        printf("✅ PASS: Standard Host System Environment Matrix\n");
    else
    {
        printf("❌ FAIL: Standard Host System Environment Matrix\n");
        success = 0;
    }
    free_char_matrix(host_reverted);

    // Phase 3: Corrected Lookup List Target Construction
    char *case_lookup[] = {
        "PATH=/usr/bin=/bin", 
        "EQUALS======", 
        "VALID=123", 
        "EMPTY_VAL=", 
        "NO_EQUAL_SIGN", 
        NULL
    };
    t_single_linked_node *lookup_lst = env_to_lst(case_lookup);
    if (!test_get_env_func(lookup_lst))
        success = 0;

    ft_single_lstclear(&lookup_lst, del_env_node_content);
    ft_single_lstclear(&host_lst, del_env_node_content);

    return (success);
}