/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_exec_command.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 13:46:00 by nilsdruon         #+#    #+#             */
/*   Updated: 2026/08/21 18:54:25 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "testing.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

typedef struct s_exec_test
{
    char    *test_name;
    char    **input;
    int     expected_status;
    char    *expected_stdout;
}   t_exec_test;

static int run_single_exec_test(t_exec_test test, char **original_envp)
{
    int         out_pipe[2];
    pid_t       pid;
    t_minishell mini;

    t_single_linked_node *env_lst = env_to_lst(original_envp);

    if (pipe(out_pipe) == -1)
    {
        perror("pipe failed");
        ft_single_lstclear(&env_lst, del_env_node_content);
        return (0);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        close(out_pipe[0]);
        close(out_pipe[1]);
        ft_single_lstclear(&env_lst, del_env_node_content);
        return (0);
    }

    if (pid == 0)
    {
        close(out_pipe[0]);
        dup2(out_pipe[1], STDOUT_FILENO);
        close(out_pipe[1]);
        
        memset(&mini, 0, sizeof(t_minishell));
        mini.prev_in = -42;
        mini.prev_out = -42;

        /* exec_command does not return; it replaces process image or exits */
        exec_command(test.input, env_lst, &mini);
        
        ft_single_lstclear(&env_lst, del_env_node_content);
        exit(mini.exit_status);
    }

    close(out_pipe[1]);

    int status;
    waitpid(pid, &status, 0);

    char out_buffer[4096];
    memset(out_buffer, 0, sizeof(out_buffer));
    read(out_pipe[0], out_buffer, sizeof(out_buffer) - 1);
    close(out_pipe[0]);

    int actual_status = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    int pass = 1;

    if (actual_status != test.expected_status)
    {
        printf("❌ FAIL: %s [Status Mismatch]\n", test.test_name);
        printf("   Expected Exit Status: %d, Got: %d\n", test.expected_status, actual_status);
        pass = 0;
    }

    if (test.expected_stdout && strstr(out_buffer, test.expected_stdout) == NULL)
    {
        if (pass) printf("❌ FAIL: %s [STDOUT Mismatch]\n", test.test_name);
        printf("   Expected stdout to contain: \"%s\"\n", test.expected_stdout);
        printf("   Got stdout                : \"%s\"\n", out_buffer);
        pass = 0;
    }

    if (pass)
        printf("✅ PASS: %s\n", test.test_name);

    ft_single_lstclear(&env_lst, del_env_node_content);
    return (pass);
}

int test_exec_command(char **envp)
{
    int success = 1;

    char *case1[] = {"/bin/echo", "hello", "world", NULL};
    char *case2[] = {"/bin/ls", NULL};
    char *case3[] = {"/bin/false", NULL};
    char *case4[] = {"/nonexistent_command", NULL};

    t_exec_test tests[] = {
        {"Execute /bin/echo", case1, 0, "hello world"},
        {"Execute /bin/ls", case2, 0, NULL},
        {"Execute /bin/false", case3, 1, NULL},
        {"Execute nonexistent command", case4, 127, NULL}
    };

    int num_tests = sizeof(tests) / sizeof(tests[0]);

    printf("\n--- Running Exec Command Tests ---\n");
    for (int i = 0; i < num_tests; i++)
    {
        if (!run_single_exec_test(tests[i], envp))
            success = 0;
    }
    return (success);
}