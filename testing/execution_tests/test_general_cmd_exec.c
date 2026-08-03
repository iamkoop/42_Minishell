/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_general_cmd_exec.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 19:20:06 by nildruon          #+#    #+#             */
/*   Updated: 2026/08/03 19:32:12 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "testing.h"

/* --- NODE CREATION HELPERS --- */

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

static t_single_linked_node *create_cmd_node(char **argv, t_single_linked_node *redir)
{
    t_single_linked_node *node = malloc(sizeof(t_single_linked_node));
    t_command *cmd = malloc(sizeof(t_command));

    if (!node || !cmd)
    {
        free(node);
        free(cmd);
        return (NULL);
    }
    cmd->argv = argv;
    cmd->redir = redir;

    node->content = cmd;
    node->next = NULL;
    return (node);
}

/* --- MEMORY CLEANUP HELPERS --- */

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

static void clear_cmd_list(t_single_linked_node **lst)
{
    t_single_linked_node *curr;
    t_single_linked_node *next;

    if (!lst || !*lst)
        return ;
    curr = *lst;
    while (curr)
    {
        next = curr->next;
        t_command *cmd = (t_command *)curr->content;
        if (cmd)
        {
            if (cmd->redir)
            {
                t_single_linked_node *r_curr = cmd->redir;
                t_single_linked_node *r_next;
                while (r_curr)
                {
                    r_next = r_curr->next;
                    free_redir_node(r_curr->content);
                    free(r_curr);
                    r_curr = r_next;
                }
            }
            free(cmd);
        }
        free(curr);
        curr = next;
    }
    *lst = NULL;
}

static void init_mock_minishell(t_minishell *mini)
{
    memset(mini, 0, sizeof(t_minishell));
    mini->redir_in = -42;
    mini->redir_out = -42;
    mini->prev_in = -42;
    mini->prev_out = -42;
    mini->in = -42;
    mini->out = -42;
    mini->prev_read_fd = -42;
}

typedef struct s_exec_test
{
    char                    *test_name;
    t_single_linked_node    *cmd_lst;
    int                     expected_exit_status;
    void                    (*setup_files)(void);
    void                    (*cleanup_files)(void);
    int                     (*assert_behavior)(t_minishell *mini);
}   t_exec_test;

/* --- TEST SETUP / CLEANUP / ASSERTIONS --- */

static void cleanup_out_files(void)
{
    unlink("exec_out1.txt");
    unlink("exec_out2.txt");
    unlink("exec_in1.txt");
}

static void setup_pipe_input(void)
{
    cleanup_out_files();
    int fd = open("exec_in1.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd >= 0)
    {
        write(fd, "hello pipe\n", 11);
        close(fd);
    }
}

static int assert_single_builtin_pwd(t_minishell *mini)
{
    int fd = open("exec_out1.txt", O_RDONLY);
    if (fd < 0) return (0);

    char buf[1024];
    memset(buf, 0, sizeof(buf));
    ssize_t bytes = read(fd, buf, sizeof(buf) - 1);
    close(fd);

    return (bytes > 0 && mini->exit_status == 0);
}

static int assert_single_external_cmd(t_minishell *mini)
{
    int fd = open("exec_out1.txt", O_RDONLY);
    if (fd < 0) return (0);

    char buf[64];
    memset(buf, 0, sizeof(buf));
    read(fd, buf, sizeof(buf) - 1);
    close(fd);

    return (strcmp(buf, "hello exec_main\n") == 0 && mini->exit_status == 0);
}

static int assert_pipeline(t_minishell *mini)
{
    int fd = open("exec_out1.txt", O_RDONLY);
    if (fd < 0) return (0);

    char buf[64];
    memset(buf, 0, sizeof(buf));
    read(fd, buf, sizeof(buf) - 1);
    close(fd);

    return (strcmp(buf, "hello pipe\n") == 0 && mini->exit_status == 0);
}

/* --- RUNNER ENGINE --- */

static int run_single_exec_test(t_exec_test *tests, int num_tests, int current_idx, t_single_linked_node *envp)
{
    pid_t           pid;
    int             pipe_fd[2];
    t_exec_test     *test = &tests[current_idx];

    if (test->setup_files)
        test->setup_files();

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe failed");
        clear_cmd_list(&test->cmd_lst);
        return (0);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        clear_cmd_list(&test->cmd_lst);
        return (0);
    }

    if (pid == 0)
    {
        close(pipe_fd[0]);
        t_minishell mini;
        init_mock_minishell(&mini);

        // Execute target core engine entry point
        exec_main(&mini, test->cmd_lst, envp);

        write(pipe_fd[1], &mini.exit_status, sizeof(int));

        int assert_res = 1;
        if (test->assert_behavior)
            assert_res = test->assert_behavior(&mini);
        write(pipe_fd[1], &assert_res, sizeof(int));

        close(pipe_fd[1]);

        for (int i = 0; i < num_tests; i++)
        {
            if (tests[i].cmd_lst)
                clear_cmd_list(&tests[i].cmd_lst);
        }
        exit(0);
    }

    close(pipe_fd[1]);

    int status;
    waitpid(pid, &status, 0);

    int actual_exit_status = -1;
    int behavior_pass = 0;

    read(pipe_fd[0], &actual_exit_status, sizeof(int));
    read(pipe_fd[0], &behavior_pass, sizeof(int));
    close(pipe_fd[0]);

    int pass = 1;

    if (actual_exit_status != test->expected_exit_status)
    {
        printf("❌ FAIL: %s [Exit Code Mismatch]\n", test->test_name);
        printf("   Expected exit_status: %d, Got: %d\n", test->expected_exit_status, actual_exit_status);
        pass = 0;
    }
    else if (test->assert_behavior && !behavior_pass)
    {
        printf("❌ FAIL: %s [Behavior Assertions Failed]\n", test->test_name);
        pass = 0;
    }

    if (pass)
        printf("✅ PASS: %s\n", test->test_name);

    if (test->cleanup_files)
        test->cleanup_files();

    clear_cmd_list(&test->cmd_lst);
    return (pass);
}

/* --- MAIN TEST SUITE INTERFACE --- */

int test_general_cmd_exec(void)
{
    int success = 1;
    printf("\n--- Running exec_main Execution Engine Suite ---\n");

    // Mock environment setup
    extern char **environ;
    t_single_linked_node *envp = env_to_lst(environ);

    t_exec_test tests[4];
    memset(tests, 0, sizeof(tests));

    /* Test Case 1: Execute single builtin with redirection (pwd > exec_out1.txt) */
    static char *pwd_argv[] = {"pwd", NULL};
    t_single_linked_node *r1 = create_redir_node("exec_out1.txt", OUT);
    tests[0].test_name = "Single Builtin with Redirection (pwd > file)";
    tests[0].cmd_lst = create_cmd_node(pwd_argv, r1);
    tests[0].expected_exit_status = 0;
    tests[0].setup_files = cleanup_out_files;
    tests[0].cleanup_files = cleanup_out_files;
    tests[0].assert_behavior = assert_single_builtin_pwd;

    /* Test Case 2: External command execution (echo "hello exec_main" > exec_out1.txt) */
    static char *echo_argv[] = {"echo", "hello exec_main", NULL};
    t_single_linked_node *r2 = create_redir_node("exec_out1.txt", OUT);
    tests[1].test_name = "Single External Command (echo text > file)";
    tests[1].cmd_lst = create_cmd_node(echo_argv, r2);
    tests[1].expected_exit_status = 0;
    tests[1].setup_files = cleanup_out_files;
    tests[1].cleanup_files = cleanup_out_files;
    tests[1].assert_behavior = assert_single_external_cmd;

    /* Test Case 3: Pipeline execution (cat exec_in1.txt | grep hello > exec_out1.txt) */
    static char *cat_argv[] = {"cat", NULL};
    static char *grep_argv[] = {"grep", "hello", NULL};
    t_single_linked_node *r3_in = create_redir_node("exec_in1.txt", IN);
    t_single_linked_node *r3_out = create_redir_node("exec_out1.txt", OUT);
    
    t_single_linked_node *cmd1 = create_cmd_node(cat_argv, r3_in);
    t_single_linked_node *cmd2 = create_cmd_node(grep_argv, r3_out);
    cmd1->next = cmd2;

    tests[2].test_name = "Pipeline Execution (cat file | grep > file)";
    tests[2].cmd_lst = cmd1;
    tests[2].expected_exit_status = 0;
    tests[2].setup_files = setup_pipe_input;
    tests[2].cleanup_files = cleanup_out_files;
    tests[2].assert_behavior = assert_pipeline;

    /* Test Case 4: Non-existent Command (returns status 127) */
    static char *invalid_argv[] = {"non_existent_command_12345", NULL};
    tests[3].test_name = "Invalid Command Handling (Return 127)";
    tests[3].cmd_lst = create_cmd_node(invalid_argv, NULL);
    tests[3].expected_exit_status = 127;

    int num_tests = sizeof(tests) / sizeof(tests[0]);
    for (int i = 0; i < num_tests; i++)
    {
        if (!run_single_exec_test(tests, num_tests, i, envp))
            success = 0;
    }

    // Fallback list memory cleanup
    for (int i = 0; i < num_tests; i++)
    {
        if (tests[i].cmd_lst)
            clear_cmd_list(&tests[i].cmd_lst);
    }
    ft_single_lstclear(&envp, del_env_node_content);

    return (success);
}