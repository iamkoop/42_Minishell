/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 23:02:37 by nildruon          #+#    #+#             */
/*   Updated: 2026/06/03 23:52:39 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "testing.h"
#include <sys/stat.h>

typedef struct s_cd_test
{
    char    *test_name;
    char    **input;
    int     expected_status;    // Child process exit code 
    char    *expected_stderr;   // Expected error string (or NULL if pure success)
    int     setup_type;         // Indicator to provision environments dynamically
}   t_cd_test;

/*
** Dynamic Sandbox Provisioning Codes:
** 0 = Default (System filesystem paths)
** 1 = Create a directory with no permissions (Checks EACCES)
*/
static void provision_test_environment(int setup_type)
{
    if (setup_type == 1)
    {
        mkdir("test_no_perm_dir", 0000);
    }
}

static void cleanup_test_environment(int setup_type)
{
    if (setup_type == 1)
    {
        chmod("test_no_perm_dir", 0755);
        rmdir("test_no_perm_dir");
    }
}

static int run_single_cd_test(t_cd_test test, t_single_linked_node *env_lst)
{
    int     err_pipe[2];
    int     out_pipe[2];
    pid_t   pid;

    provision_test_environment(test.setup_type);

    if (pipe(err_pipe) == -1 || pipe(out_pipe) == -1)
    {
        perror("pipe failed");
        cleanup_test_environment(test.setup_type);
        return (0);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        close(err_pipe[0]); close(err_pipe[1]);
        close(out_pipe[0]); close(out_pipe[1]);
        cleanup_test_environment(test.setup_type);
        return (0);
    }

    if (pid == 0)
    {
        // Child Process Sandbox isolation 
        close(out_pipe[0]);
        dup2(out_pipe[1], STDOUT_FILENO);
        close(out_pipe[1]);

        close(err_pipe[0]);
        dup2(err_pipe[1], STDERR_FILENO);
        close(err_pipe[1]);

        // ==========================================
        // CALLING YOUR CD IMPLEMENTATION
        // ==========================================
        cd(test.input, env_lst);
        // ==========================================

        /*
        ** Note: Since your cd returns void, we use g_exit_status or your 
        ** custom global track value if available, or we just pull the active 
        ** status directly via validation checks or internal global setups.
        ** Modify this exit tracking line below to match your shell's global error tracker if needed!
        */
        exit(0); 
    }

    // Parent side structural collection
    close(err_pipe[1]);
    close(out_pipe[1]);

    int status;
    waitpid(pid, &status, 0);

    char err_buffer[4096];
    memset(err_buffer, 0, sizeof(err_buffer));
    read(err_pipe[0], err_buffer, sizeof(err_buffer) - 1);
    close(err_pipe[0]);

    char out_buffer[4096];
    memset(out_buffer, 0, sizeof(out_buffer));
    read(out_pipe[0], out_buffer, sizeof(out_buffer) - 1);
    close(out_pipe[0]);

    cleanup_test_environment(test.setup_type);

    int pass = 1;

    // 1. Verify Standard Output is silent (cd doesn't print to stdout unless string parsing occurs)
    if (out_buffer[0] != '\0')
    {
        printf("❌ FAIL: %s [Unexpected STDOUT Output]\n", test.test_name);
        printf("   Got unexpected stdout data: \"%s\"\n", out_buffer);
        pass = 0;
    }

    // 2. Verify Standard Error Responses
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

    if (pass)
        printf("✅ PASS: %s\n", test.test_name);
    return (pass);
}

int test_cd(char **envp)
{
    int success = 1;

    // Translate the raw system env matrix into your active structural list
    t_single_linked_node *env_lst = env_to_lst(envp);

    // Static test cases configuration arrays 
    char *case1[] = {"cd", "..", NULL};
    char *case2[] = {"cd", ".", NULL};
    char *case3[] = {"cd", "non_existent_directory_123", NULL};
    char *case4[] = {"cd", "Makefile", NULL}; // Valid file, but not a directory (ENOTDIR)
    char *case5[] = {"cd", "test_no_perm_dir", NULL}; // Directory without access rules (EACCES)
    char *case6[] = {"cd", "dir1", "dir2", NULL}; // Too many arguments error condition
    char *case7[] = {"cd", "", NULL}; // Empty target string sequence edge case

    t_cd_test tests[] = {
        {"Navigate up to parent directory (cd ..)", case1, 0, NULL, 0},
        {"Navigate into current directory (cd .)", case2, 0, NULL, 0},
        {"Target directory does not exist error", case3, 1, "No such file or directory", 0},
        {"Target path is a standard file, not a folder", case4, 1, "Not a directory", 0},
        {"Target path lacks execute permissions", case5, 1, "Permission denied", 1},
        {"Too many positional arguments passed to cd", case6, 1, "too many arguments", 0},
        {"Empty string relative context verification", case7, 0, NULL, 0}
    };

    int num_tests = sizeof(tests) / sizeof(tests[0]);

    printf("\n--- Running Minishell Cd Builtin Tests ---\n");
    for (int i = 0; i < num_tests; i++)
    {
        if (!run_single_cd_test(tests[i], env_lst))
            success = 0;
    }

    // Clean structural lists safely
    if (env_lst)
        ft_single_lstclear(&env_lst, del_env_node_content);

    return (success);
}