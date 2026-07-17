/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 15:57:58 by nildruon          #+#    #+#             */
/*   Updated: 2026/07/17 14:32:28 by nildruon         ###   ########.fr       */
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
    t_single_linked_node *curr;
    t_single_linked_node *next;

    if (!lst || !*lst)
        return ;
    curr = *lst;
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
    int                     expected_return;
    void                    (*setup_files)(void);
    void                    (*cleanup_files)(void);
    int                     (*assert_behavior)(t_minishell *mini);
}   t_redir_test;

/* --- TEST SETUP / CLEANUP FUNCTIONS --- */

static void setup_basic_in_out(void)
{
    int fd = open("test_infile1.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd >= 0)
    {
        write(fd, "first", 5);
        close(fd);
    }
    fd = open("test_infile2.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd >= 0)
    {
        write(fd, "second", 6);
        close(fd);
    }
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
    int fd = open("test_readonly.txt", O_CREAT | O_WRONLY, 0444);
    if (fd >= 0)
        close(fd);
}

static void cleanup_read_only_err(void)
{
    cleanup_basic_in_out();
    unlink("test_readonly.txt");
}

/* --- ASSERTION HELPERS --- */

static int assert_append(t_minishell *mini)
{
    int fd = open("test_outfile1.txt", O_RDONLY);
    if (fd < 0) return (0);
    
    char buf[64];
    memset(buf, 0, sizeof(buf));
    read(fd, buf, sizeof(buf) - 1);
    close(fd);
    
    return (mini->final_redir_out >= 0);
}

static int assert_multiple_inputs(t_minishell *mini)
{
    if (mini->final_redir_in < 0)
        return (0);
    
    char buf[10];
    memset(buf, 0, 10);
    
    // Read directly at offset 0 without altering the pointer stream index
    if (pread(mini->final_redir_in, buf, 9, 0) < 0)
        return (0);
    
    return (strcmp(buf, "second") == 0);
}

static int assert_multiple_outputs(t_minishell *mini)
{
    int file1_exists = (access("test_outfile1.txt", F_OK) == 0);
    int file2_exists = (access("test_outfile2.txt", F_OK) == 0);
    
    return (file1_exists && file2_exists && mini->final_redir_out >= 0);
}

/* --- RUN TEST IN CHILD PROCESS --- */

static int run_single_redir_test(t_redir_test *tests, int num_tests, int current_idx)
{
    pid_t           pid;
    int             pipe_fd[2];
    t_redir_test    *test = &tests[current_idx];

    if (test->setup_files)
        test->setup_files();

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe failed");
        clear_redir_list(&test->redir_lst);
        return (0);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        clear_redir_list(&test->redir_lst);
        return (0);
    }

    if (pid == 0)
    {
        close(pipe_fd[0]);
        t_minishell mini;
        init_mock_minishell(&mini);

        int ret = redirections(test->redir_lst, &mini);

        write(pipe_fd[1], &ret, sizeof(int));
        write(pipe_fd[1], &mini.final_redir_in, sizeof(int));
        write(pipe_fd[1], &mini.final_redir_out, sizeof(int));
        
        int assert_res = 1;
        if (test->assert_behavior)
            assert_res = test->assert_behavior(&mini);
        write(pipe_fd[1], &assert_res, sizeof(int));

        close(pipe_fd[1]);
        
        // Clean up every list allocation copied inside this child's memory space
        for (int i = 0; i < num_tests; i++)
        {
            if (tests[i].redir_lst)
                clear_redir_list(&tests[i].redir_lst);
        }
        
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

    if (actual_ret != test->expected_return)
    {
        printf("❌ FAIL: %s [Return Mismatch]\n", test->test_name);
        printf("   Expected return: %d, Got: %d\n", test->expected_return, actual_ret);
        pass = 0;
    }
    else if (actual_ret == 0)
    {
        pass = 1; 
    }
    else if (test->assert_behavior && !behavior_pass)
    {
        printf("❌ FAIL: %s [Behavior Assertions Failed]\n", test->test_name);
        printf("   Resulting final_redir_in: %d, final_redir_out: %d\n", final_in, final_out);
        pass = 0;
    }

    if (pass)
        printf("✅ PASS: %s\n", test->test_name);

    if (test->cleanup_files)
        test->cleanup_files();

    clear_redir_list(&test->redir_lst);
    return (pass);
}

int test_redirections_suite(void)
{
    int success = 1;
    printf("\n--- Running Redirection Engine Tests ---\n");

    t_redir_test tests[6];
    memset(tests, 0, sizeof(tests));

    /* Test Case 1: Simple input & output redirection */
    tests[0].test_name = "Simple Redirection (IN + OUT)";
    tests[0].redir_lst = create_redir_node("test_infile1.txt", IN);
    if (tests[0].redir_lst) 
        tests[0].redir_lst->next = create_redir_node("test_outfile1.txt", OUT);
    tests[0].expected_return = 1;
    tests[0].setup_files = setup_basic_in_out;
    tests[0].cleanup_files = cleanup_basic_in_out;

    /* Test Case 2: Multiple inputs */
    tests[1].test_name = "Multiple Inputs (Last overrides)";
    tests[1].redir_lst = create_redir_node("test_infile1.txt", IN);
    if (tests[1].redir_lst)
        tests[1].redir_lst->next = create_redir_node("test_infile2.txt", IN);
    tests[1].expected_return = 1;
    tests[1].setup_files = setup_basic_in_out;
    tests[1].cleanup_files = cleanup_basic_in_out;
    tests[1].assert_behavior = assert_multiple_inputs;

    /* Test Case 3: Multiple outputs */
    tests[2].test_name = "Multiple Outputs (All created, last retained)";
    tests[2].redir_lst = create_redir_node("test_outfile1.txt", OUT);
    if (tests[2].redir_lst)
        tests[2].redir_lst->next = create_redir_node("test_outfile2.txt", OUT);
    tests[2].expected_return = 1;
    tests[2].setup_files = setup_basic_in_out;
    tests[2].cleanup_files = cleanup_basic_in_out;
    tests[2].assert_behavior = assert_multiple_outputs;

    /* Test Case 4: Output Append mode */
    tests[3].test_name = "Append Mode (APPEND)";
    tests[3].redir_lst = create_redir_node("test_outfile1.txt", APPEND);
    tests[3].expected_return = 1;
    tests[3].setup_files = setup_basic_in_out;
    tests[3].cleanup_files = cleanup_basic_in_out;
    tests[3].assert_behavior = assert_append;

    /* Test Case 5: Error handling (Input file does not exist) */
    tests[4].test_name = "Non-existent Input File Error";
    tests[4].redir_lst = create_redir_node("non_existent_file.txt", IN);
    tests[4].expected_return = 0;

    /* Test Case 6: Error handling (Permission denied on output) */
    tests[5].test_name = "Write Permission Denied Error";
    tests[5].redir_lst = create_redir_node("test_readonly.txt", OUT);
    tests[5].expected_return = 0;
    tests[5].setup_files = setup_read_only_err;
    tests[5].cleanup_files = cleanup_read_only_err;

    int num_tests = sizeof(tests) / sizeof(tests[0]);
    for (int i = 0; i < num_tests; i++)
    {
        if (!run_single_redir_test(tests, num_tests, i))
            success = 0;
    }

    // Secondary fallback cleanup to catch allocated elements in case of upstream runner panics
    for (int i = 0; i < num_tests; i++)
    {
        if (tests[i].redir_lst)
            clear_redir_list(&tests[i].redir_lst);
    }

    return (success);
}