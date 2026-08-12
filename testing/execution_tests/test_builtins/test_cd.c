/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 23:02:37 by nildruon          #+#    #+#             */
/*   Updated: 2026/06/10 00:46:54 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "testing.h"

typedef struct s_cd_test
{
    char    *test_name;
    char    **input;
    int     expected_status;    
    char    *expected_stderr;   
    int     setup_type;         
}   t_cd_test;

static void unset_env_variable(const char *key, t_single_linked_node **env_lst)
{
    t_single_linked_node *curr = *env_lst;
    t_single_linked_node *prev = NULL;

    while (curr)
    {
        t_env_var *var = (t_env_var *)curr->content;
        if (strcmp(var->key, key) == 0)
        {
            if (prev == NULL)
                *env_lst = curr->next;
            else
                prev->next = curr->next;
            free(var->key);
            free(var->value);
            free(var);
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

static void provision_test_environment(int setup_type, char *saved_cwd, t_single_linked_node **env_lst)
{
    getcwd(saved_cwd, 4096);
    if (setup_type == 1)
    {
        mkdir("test_no_perm_dir", 0000);
    }
    else if (setup_type == 2 || setup_type == 3)
    {
        mkdir("test_rm_dir", 0755);
        chdir("test_rm_dir");
        mkdir("sub_dir", 0755);
        chdir("sub_dir");
        rmdir("../sub_dir");
        rmdir("../../test_rm_dir");
        
        if (setup_type == 3)
            unset_env_variable("PWD", env_lst);
    }
    else if (setup_type == 4)
    {
        unset_env_variable("HOME", env_lst);
    }
    else if (setup_type == 5)
    {
        t_single_linked_node *curr = *env_lst;
        while (curr)
        {
            t_env_var *var = (t_env_var *)curr->content;
            if (strcmp(var->key, "HOME") == 0)
            {
                free(var->value);
                var->value = strdup("");
                break;
            }
            curr = curr->next;
        }
    }
    else if (setup_type == 6)
    {
        unset_env_variable("OLDPWD", env_lst);
    }
    else if (setup_type == 7)
    {
        mkdir("test_sym_target", 0755);
        symlink("test_sym_target", "test_broken_link");
        rmdir("test_sym_target");
    }
}

static void cleanup_test_environment(int setup_type, const char *saved_cwd)
{
    if (setup_type == 1)
    {
        chmod("test_no_perm_dir", 0755);
        rmdir("test_no_perm_dir");
    }
    else if (setup_type == 7)
    {
        unlink("test_broken_link");
    }
    chdir(saved_cwd);
}

static int run_single_cd_test(t_cd_test test, char **original_envp)
{
    char    saved_cwd[4096];
    int     saved_stderr;
    int     err_pipe[2];
    char    err_buffer[4096];
    int     actual_status;
    int     pass = 1;

    t_single_linked_node *env_lst = env_to_lst(original_envp);

    provision_test_environment(test.setup_type, saved_cwd, &env_lst);

    if (pipe(err_pipe) == -1)
    {
        perror("pipe failed");
        ft_single_lstclear(&env_lst, del_env_node_content);
        cleanup_test_environment(test.setup_type, saved_cwd);
        return (0);
    }
    saved_stderr = dup(STDERR_FILENO);
    dup2(err_pipe[1], STDERR_FILENO);
    close(err_pipe[1]);

    actual_status = cd(test.input, env_lst);

    fflush(stderr);
    dup2(saved_stderr, STDERR_FILENO);
    close(saved_stderr);

    memset(err_buffer, 0, sizeof(err_buffer));
    fcntl(err_pipe[0], F_SETFL, O_NONBLOCK);
    read(err_pipe[0], err_buffer, sizeof(err_buffer) - 1);
    close(err_pipe[0]);

    if (actual_status != test.expected_status)
    {
        printf("❌ FAIL: %s [Exit Status Mismatch]\n", test.test_name);
        printf("   Expected status: %d, Got: %d\n", test.expected_status, actual_status);
        pass = 0;
    }

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
    else if (err_buffer[0] != '\0' && test.setup_type != 2 && test.setup_type != 3)
    {
        if (pass) printf("❌ FAIL: %s [Unexpected Error Output]\n", test.test_name);
        printf("   Got unexpected stderr noise: \"%s\"\n", err_buffer);
        pass = 0;
    }

    if (pass)
        printf("✅ PASS: %s\n", test.test_name);

    cleanup_test_environment(test.setup_type, saved_cwd);
    ft_single_lstclear(&env_lst, del_env_node_content);
    return (pass);
}

int test_cd(char **envp)
{
    int success = 1;

    char *case1[] = {"cd", "..", NULL};
    char *case2[] = {"cd", ".", NULL};
    char *case3[] = {"cd", "non_existent_directory_123", NULL};
    char *case4[] = {"cd", "Makefile", NULL};             
    char *case5[] = {"cd", "test_no_perm_dir", NULL};     
    char *case6[] = {"cd", "dir1", "dir2", NULL};         
    char *case7[] = {"cd", "", NULL};                     
    char *case8[] = {"cd", "..", NULL};                   
    char *case9[] = {"cd", "..", NULL};                   
    char *case10[] = {"cd", NULL};                        
    char *case11[] = {"cd", NULL};                        
    char *case12[] = {"cd", "-", NULL};                   // cd - when OLDPWD is missing
    char *case13[] = {"cd", "test_broken_link", NULL};   // cd targeting a broken symlink
    char *case14[] = {"cd", "../../../../../../../../../../../../../../../..", NULL}; // Deep root overflow fallback check

    t_cd_test tests[] = {
        {"Navigate up to parent directory (cd ..)", case1, 0, NULL, 0},
        {"Navigate into current directory (cd .)", case2, 0, NULL, 0},
        {"Target directory does not exist error", case3, 1, "No such file or directory", 0},
        {"Target path is a standard file, not a folder", case4, 1, "Not a directory", 0},
        {"Target path lacks execute permissions", case5, 1, "Permission denied", 1},
        {"Too many positional arguments passed to cd", case6, 1, "too many arguments", 0},
        {"Empty string relative context verification", case7, 0, NULL, 0},
        {"Edge Case: CD up when current directory is deleted", case8, 0, "error retrieving current directory", 2},
        {"Destructive: CD up when PWD is UNSET and directory is deleted", case9, 0, "error retrieving current directory", 3},
        {"Environment: Missing HOME error response parsing", case10, 1, "HOME not set", 4},
        {"Environment: Empty HOME string route parsing", case11, 1, "No such file or directory", 5},
        {"Environment: Missing OLDPWD error handling (cd -)", case12, 1, "OLDPWD not set", 6},
        {"Filesystem: Reject navigating through broken symlinks", case13, 1, "No such file or directory", 7},
        {"Robustness: Deep directory traversal string containment", case14, 0, NULL, 0}
    };

    int num_tests = sizeof(tests) / sizeof(tests[0]);

    printf("\n--- Running Extended Minishell Cd Builtin Tests ---\n");
    for (int i = 0; i < num_tests; i++)
    {
        if (!run_single_cd_test(tests[i], envp))
            success = 0;
    }

    return (success);
}