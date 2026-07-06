/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 12:09:29 by nilsdruon         #+#    #+#             */
/*   Updated: 2026/07/06 13:31:29 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "testing.h"

typedef struct s_pwd_test
{
    char    *test_name;
    char    **input;
    int     expected_status;
    char    *expected_stderr;   // Set to NULL if no error output is expected
}   t_pwd_test;

static int run_single_pwd_test(t_pwd_test test, const char *system_cwd)
{
    int     out_pipe[2];
    int     err_pipe[2];
    pid_t   pid;

    if (pipe(out_pipe) == -1 || pipe(err_pipe) == -1)
    {
        perror("pipe failed");
        return (0);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        close(out_pipe[0]); close(out_pipe[1]);
        close(err_pipe[0]); close(err_pipe[1]);
        return (0);
    }

    if (pid == 0)
    {
        close(out_pipe[0]);
        dup2(out_pipe[1], STDOUT_FILENO);
        close(out_pipe[1]);

        close(err_pipe[0]);
        dup2(err_pipe[1], STDERR_FILENO);
        close(err_pipe[1]);

        // CALLING YOUR PWD IMPLEMENTATION
        int ret = pwd(test.input);

        exit(ret);
    }

    close(out_pipe[1]);
    close(err_pipe[1]);

    int status;
    waitpid(pid, &status, 0);

    char out_buffer[4098];
    memset(out_buffer, 0, sizeof(out_buffer));
    read(out_pipe[0], out_buffer, sizeof(out_buffer) - 1);
    close(out_pipe[0]);

    char err_buffer[4096];
    memset(err_buffer, 0, sizeof(err_buffer));
    read(err_pipe[0], err_buffer, sizeof(err_buffer) - 1);
    close(err_pipe[0]);

    int actual_status = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    int pass = 1;

    // 1. Verify Exit Code
    if (actual_status != test.expected_status)
    {
        printf("❌ FAIL: %s [Status Mismatch]\n", test.test_name);
        printf("   Expected Exit Status: %d, Got: %d\n", test.expected_status, actual_status);
        pass = 0;
    }

    // 2. Verify Standard Output Text (Only check if we expect success)
    if (test.expected_stderr == NULL)
    {
        char expected_stdout[4098];
        memset(expected_stdout, 0, sizeof(expected_stdout));
        snprintf(expected_stdout, sizeof(expected_stdout), "%s\n", system_cwd);

        if (strcmp(out_buffer, expected_stdout) != 0)
        {
            if (pass) printf("❌ FAIL: %s [STDOUT Mismatch]\n", test.test_name);
            printf("   Expected: \"%s\"\n", expected_stdout);
            printf("   Got     : \"%s\"\n", out_buffer);
            pass = 0;
        }
    }
    else
    {
        // If an error happened, standard output should generally be clean/empty
        if (out_buffer[0] != '\0')
        {
            if (pass) printf("❌ FAIL: %s [Unexpected STDOUT Output on Error]\n", test.test_name);
            printf("   Got unexpected stdout noise: \"%s\"\n", out_buffer);
            pass = 0;
        }
    }

    // 3. Verify Standard Error (Stderr) Output
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

int test_pwd(void)
{
    int success = 1;

    char *case1[] = {"pwd", NULL};
    char *case2[] = {"pwd", "kek", NULL};
    char *case3[] = {"pwd", "lel", "-r", NULL};
    char *case4[] = {"pwd", "kek", "-F", "-F", NULL};
    char *case5[] = {"pwd", "-f", "-n", "-F", NULL};
    char *case6[] = {"pwd", "-f", "kek", "Yolo", NULL};

    t_pwd_test tests[] = {
        {"Standard pwd execution", case1, 0, NULL},
        {"Pwd with a trailing word argument", case2, 0, NULL},
        {"Pwd with trailing words and fake flags", case3, 0, NULL},
        {"Pwd with multiple trailing masked arguments", case4, 0, NULL},
        {"Pwd with invalid flag option", case5, 2, "invalid option"},
        {"Pwd with invalid flag option followed by strings", case6, 2, "invalid option"}
    };

    int num_tests = sizeof(tests) / sizeof(tests[0]);

    char system_cwd[4096];
    memset(system_cwd, 0, sizeof(system_cwd));
    if (!getcwd(system_cwd, sizeof(system_cwd) - 1))
    {
        perror("getcwd failed inside test suite");
        return (0);
    }

    printf("\n--- Running Minishell Pwd Builtin Tests ---\n");
    for (int i = 0; i < num_tests; i++)
    {
        if (!run_single_pwd_test(tests[i], system_cwd))
            success = 0;
    }
    return (success);
}