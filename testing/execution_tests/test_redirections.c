/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 15:57:58 by nildruon          #+#    #+#             */
/*   Updated: 2026/07/16 18:05:50 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "testing.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

static t_single_linked_node *create_redir_node(char *filename, t_redir_type type)
{
    t_single_linked_node *node = malloc(sizeof(t_single_linked_node));
    t_redir_list *content = malloc(sizeof(t_redir_list));
    
    if (!node || !content)
    {
        free(node);
        free(content);
        return (NULL);
    }
    content->filename = filename ? strdup(filename) : NULL;
    content->fd = -42;
    content->redir_type = type;
    
    node->content = content;
    node->next = NULL;
    return (node);
}

/* Helper to clean up the allocated redirection list */
static void free_redir_node(void *content)
{
    t_redir_list *redir = (t_redir_list *)content;
    if (redir)
    {
        if (redir->filename)
            free(redir->filename);
        if (redir->fd >= 0)
            close(redir->fd);
        free(redir);
    }
}

/* Clear helper matching libft list clear signature */
static void clear_redir_list(t_single_linked_node **lst)
{
    t_single_linked_node *curr = *lst;
    t_single_linked_node *next;

    while (curr)
    {
        next = curr->next;
        free_redir_node(curr->content);
        free(curr);
        curr = next;
    }
    *lst = NULL;
}

/* Setup standard mock minishell state */
static void init_mock_minishell(t_minishell *mini)
{
    memset(mini, 0, sizeof(t_minishell));
    mini->final_redir_in = -42;
    mini->final_redir_out = -42;
    mini->prev_in = -42;
    mini->prev_out = -42;
    mini->in = -42;
    mini->out = -42;
}

typedef struct s_redir_test
{
    char                    *test_name;
    t_single_linked_node    *redir_lst;
    int                     expected_return; // 1 for Success (true), 0 for Failure (false)
    void                    (*setup_files)(void);
    void                    (*cleanup_files)(void);
    int                     (*assert_behavior)(t_minishell *mini);
}   t_redir_test;

/* --- TEST SETUP / CLEANUP FUNCTIONS --- */

static void setup_basic_in_out(void)
{
    int fd = open("test_infile1.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    write(fd, "first", 5);
    close(fd);
    fd = open("test_infile2.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    write(fd, "second", 6);
    close(fd);
    unlink("test_outfile1.txt");
    unlink("test_outfile2.txt");
}

static void cleanup_basic_in_out(void)
{
    unlink("test_infile1.txt");
    unlink("test_infile2.txt");
    unlink("test_outfile1.txt");
    unlink("test_outfile2.txt");
}

static void setup_read_only_err(void)
{
    setup_basic_in_out();
    // Create a read-only file to trigger permission issues for writing
    int fd = open("test_readonly.txt", O_CREAT | O_WRONLY, 0444);
    close(fd);
}

static void cleanup_read_only_err(void)
{
    cleanup_basic_in_out();
    unlink("test_readonly.txt");
}

/* --- ASSERTION HELPERS --- */

// Assert that the final out file has the appended content
static int assert_append(t_minishell *mini)
{
    (void)mini;
    int fd = open("test_outfile1.txt", O_RDONLY);
    if (fd < 0) return (0);
    
    char buf[64];
    memset(buf, 0, sizeof(buf));
    read(fd, buf, sizeof(buf) - 1);
    close(fd);
    
    return (mini->final_redir_out >= 0);
}

// Assert that multiple input files were opened, but final_redir_in targets the last one
static int assert_multiple_inputs(t_minishell *mini)
{
    if (mini->final_redir_in < 0)
        return (0);
    
    char buf[10];
    memset(buf, 0, 10);
    read(mini->final_redir_in, buf, 9);
    
    return (strcmp(buf, "second") == 0);
}

// Assert that both output files were touched/created, but only the last one is held open
static int assert_multiple_outputs(t_minishell *mini)
{
    int file1_exists = (access("test_outfile1.txt", F_OK) == 0);
    int file2_exists = (access("test_outfile2.txt", F_OK) == 0);
    
    return (file1_exists && file2_exists && mini->final_redir_out >= 0);
}

/* --- RUN TEST IN CHILD PROCESS --- */

static int run_single_redir_test(t_redir_test test)
{
    pid_t   pid;
    int     pipe_fd[2];

    if (test.setup_files)
        test.setup_files();

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe failed");
        return (0);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return (0);
    }

    if (pid == 0)
    {
        close(pipe_fd[0]);
        t_minishell mini;
        init_mock_minishell(&mini);

        int ret = redirections(test.redir_lst, &mini);

        // Send back the crucial structural results over the pipe
        write(pipe_fd[1], &ret, sizeof(int));
        write(pipe_fd[1], &mini.final_redir_in, sizeof(int));
        write(pipe_fd[1], &mini.final_redir_out, sizeof(int));
        
        // Assertions that need file checks inside child context
        int assert_res = 1;
        if (test.assert_behavior)
            assert_res = test.assert_behavior(&mini);
        write(pipe_fd[1], &assert_res, sizeof(int));

        close(pipe_fd[1]);
        
        // Cleanup list in child before exiting
        clear_redir_list(&test.redir_lst);
        exit(0);
    }

    close(pipe_fd[1]);

    int status;
    waitpid(pid, &status, 0);

    int actual_ret = -1;
    int final_in = -42;
    int final_out = -42;
    int behavior_pass = 0;

    read(pipe_fd[0], &actual_ret, sizeof(int));
    read(pipe_fd[0], &final_in, sizeof(int));
    read(pipe_fd[0], &final_out, sizeof(int));
    read(pipe_fd[0], &behavior_pass, sizeof(int));
    close(pipe_fd[0]);

    int pass = 1;

    // 1. Check if the function's exit status matches what we expected (0/false or 1/true)
    if (actual_ret != test.expected_return)
    {
        printf("❌ FAIL: %s [Return Mismatch]\n", test.test_name);
        printf("   Expected return: %d, Got: %d\n", test.expected_return, actual_ret);
        pass = 0;
    }
    // 2. If it returned a matched failure (0/false), then error handling worked correctly
    else if (actual_ret == 0)
    {
        pass = 1;
    }
    // 3. If it succeeded (1/true), verify that structural integrity/post-conditions pass
    else if (test.assert_behavior && !behavior_pass)
    {
        printf("❌ FAIL: %s [Behavior Assertions Failed]\n", test.test_name);
        printf("   Resulting final_redir_in: %d, final_redir_out: %d\n", final_in, final_out);
        pass = 0;
    }

    if (pass)
        printf("✅ PASS: %s\n", test.test_name);

    if (test.cleanup_files)
        test.cleanup_files();

    clear_redir_list(&test.redir_lst);
    return (pass);
}

int test_redirections_suite(void)
{
    int success = 1;
    printf("\n--- Running Redirection Engine Tests ---\n");

    /* Test Case 1: Simple input & output redirection */
    t_single_linked_node *lst1 = create_redir_node("test_infile1.txt", IN);
    lst1->next = create_redir_node("test_outfile1.txt", OUT);
    
    /* Test Case 2: Multiple inputs (Only the last input file descriptor should persist in final_redir_in) */
    t_single_linked_node *lst2 = create_redir_node("test_infile1.txt", IN);
    lst2->next = create_redir_node("test_infile2.txt", IN);

    /* Test Case 3: Multiple outputs (All files created, only last remains open in final_redir_out) */
    t_single_linked_node *lst3 = create_redir_node("test_outfile1.txt", OUT);
    lst3->next = create_redir_node("test_outfile2.txt", OUT);

    /* Test Case 4: Output Append mode */
    t_single_linked_node *lst4 = create_redir_node("test_outfile1.txt", APPEND);

    /* Test Case 5: Error handling (Input file does not exist) */
    t_single_linked_node *lst5 = create_redir_node("non_existent_file.txt", IN);

    /* Test Case 6: Error handling (Permission denied on output) */
    t_single_linked_node *lst6 = create_redir_node("test_readonly.txt", OUT);

    t_redir_test tests[] = {
        {
            .test_name = "Simple Redirection (IN + OUT)",
            .redir_lst = lst1,
            .expected_return = 1, // Expect true (1) on success
            .setup_files = setup_basic_in_out,
            .cleanup_files = cleanup_basic_in_out,
            .assert_behavior = NULL
        },
        {
            .test_name = "Multiple Inputs (Last overrides)",
            .redir_lst = lst2,
            .expected_return = 1, // Expect true (1) on success
            .setup_files = setup_basic_in_out,
            .cleanup_files = cleanup_basic_in_out,
            .assert_behavior = assert_multiple_inputs
        },
        {
            .test_name = "Multiple Outputs (All created, last retained)",
            .redir_lst = lst3,
            .expected_return = 1, // Expect true (1) on success
            .setup_files = setup_basic_in_out,
            .cleanup_files = cleanup_basic_in_out,
            .assert_behavior = assert_multiple_outputs
        },
        {
            .test_name = "Append Mode (APPEND)",
            .redir_lst = lst4,
            .expected_return = 1, // Expect true (1) on success
            .setup_files = setup_basic_in_out,
            .cleanup_files = cleanup_basic_in_out,
            .assert_behavior = assert_append
        },
        {
            .test_name = "Non-existent Input File Error",
            .redir_lst = lst5,
            .expected_return = 0, // Expect false (0) on failure
            .setup_files = NULL,
            .cleanup_files = NULL,
            .assert_behavior = NULL
        },
        {
            .test_name = "Write Permission Denied Error",
            .redir_lst = lst6,
            .expected_return = 0, // Expect false (0) on failure
            .setup_files = setup_read_only_err,
            .cleanup_files = cleanup_read_only_err,
            .assert_behavior = NULL
        }
    };

    int num_tests = sizeof(tests) / sizeof(tests[0]);
    for (int i = 0; i < num_tests; i++)
    {
        if (!run_single_redir_test(tests[i]))
            success = 0;
    }
    return (success);
}