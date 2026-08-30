/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_general_cmd_exec.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 19:55:38 by username          #+#    #+#             */
/*   Updated: 2026/08/30 17:18:35 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "testing.h"

/* --- FORWARD DECLARATIONS --- */

static void	free_redir_node(void *content);
static void	clear_cmd_list(t_single_linked_node **lst);

/* --- NODE CREATION HELPERS --- */

static t_single_linked_node	*create_redir_node(char *filename, enum e_redir_type type)
{
	t_single_linked_node	*node;
	t_redir_list			*content;

	node = ft_calloc(1, sizeof(t_single_linked_node));
	content = ft_calloc(1, sizeof(t_redir_list));
	if (!node || !content)
	{
		free(node);
		free(content);
		return (NULL);
	}
	content->filename = filename ? strdup(filename) : NULL;
	if (filename && !content->filename)
	{
		free(content);
		free(node);
		return (NULL);
	}
	content->fd = -42;
	content->redir_type = type;
	node->content = content;
	node->next = NULL;
	return (node);
}

static char	**clone_argv(char **argv)
{
	int	count = 0;

	if (!argv)
		return (NULL);
	while (argv[count])
		count++;
	char	**copy = ft_calloc(count + 1, sizeof(char *));

	if (!copy)
		return (NULL);
	for (int i = 0; i < count; i++)
	{
		copy[i] = strdup(argv[i]);
		if (!copy[i])
		{
			for (int j = 0; j < i; j++)
				free(copy[j]);
			free(copy);
			return (NULL);
		}
	}
	return (copy);
}

static t_single_linked_node	*create_cmd_node(char **argv, t_single_linked_node *redir)
{
	t_single_linked_node	*node;
	t_command				*cmd;

	node = ft_calloc(1, sizeof(t_single_linked_node));
	cmd = ft_calloc(1, sizeof(t_command));
	if (!node || !cmd)
	{
		free(node);
		free(cmd);
		return (NULL);
	}
	cmd->argv = clone_argv(argv);
	if (argv && !cmd->argv)
	{
		free(cmd);
		free(node);
		return (NULL);
	}
	cmd->redir = redir;
	node->content = cmd;
	node->next = NULL;
	return (node);
}

/* --- DEEP COPY HELPERS (ISOLATES MINI FROM TEST DEF) --- */

static t_single_linked_node	*clone_redir_list(t_single_linked_node *redir_src)
{
	t_single_linked_node	*head = NULL;
	t_single_linked_node	*tail = NULL;
	t_single_linked_node	*curr = redir_src;

	while (curr)
	{
		t_redir_list			*src_r = (t_redir_list *) curr->content;
		t_single_linked_node	*new_node = create_redir_node(src_r->filename, src_r->redir_type);

		if (!new_node)
		{
			while (head)
			{
				t_single_linked_node	*tmp = head->next;

				free_redir_node(head->content);
				free(head);
				head = tmp;
			}
			return (NULL);
		}
		if (!head)
			head = new_node;
		else
			tail->next = new_node;
		tail = new_node;
		curr = curr->next;
	}
	return (head);
}

static t_single_linked_node	*clone_cmd_list(t_single_linked_node *src)
{
	t_single_linked_node	*head = NULL;
	t_single_linked_node	*tail = NULL;
	t_single_linked_node	*curr = src;

	while (curr)
	{
		t_command				*src_cmd = (t_command *) curr->content;
		t_single_linked_node	*cloned_redirs = clone_redir_list(src_cmd->redir);
		t_single_linked_node	*new_cmd_node = create_cmd_node(src_cmd->argv, cloned_redirs);

		if (!new_cmd_node)
		{
			clear_cmd_list(&head);
			return (NULL);
		}
		if (!head)
			head = new_cmd_node;
		else
			tail->next = new_cmd_node;
		tail = new_cmd_node;
		curr = curr->next;
	}
	return (head);
}

/* --- MEMORY CLEANUP HELPERS --- */

static void	free_redir_node(void *content)
{
	t_redir_list	*redir = (t_redir_list *) content;

	if (redir)
	{
		if (redir->filename)
			free(redir->filename);
		if (redir->fd >= 0)
			close(redir->fd);
		free(redir);
	}
}

static void	clear_cmd_list(t_single_linked_node **lst)
{
	t_single_linked_node	*curr;
	t_single_linked_node	*next;

	if (!lst || !*lst)
		return ;
	curr = *lst;
	while (curr)
	{
		next = curr->next;
		t_command	*cmd = (t_command *) curr->content;

		if (cmd)
		{
			if (cmd->argv)
			{
				for (int i = 0; cmd->argv[i]; i++)
					free(cmd->argv[i]);
				free(cmd->argv);
			}
			if (cmd->redir)
			{
				t_single_linked_node	*r_curr = cmd->redir;
				t_single_linked_node	*r_next;

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

static void	init_mock_minishell(t_minishell *mini)
{
	ft_bzero(mini, sizeof(t_minishell));
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
	char					*test_name;
	t_single_linked_node	*cmd_lst;
	int						expected_exit_status;
	void	(*setup_files)(void);
	void	(*cleanup_files)(void);
	int	(*assert_behavior)(t_minishell *mini);
}	t_exec_test;

/* --- TEST SETUP / CLEANUP / ASSERTIONS --- */

static void	cleanup_out_files(void)
{
	unlink("exec_out1.txt");
	unlink("exec_out2.txt");
}

static void	cleanup_all_files(void)
{
	unlink("exec_out1.txt");
	unlink("exec_out2.txt");
	unlink("exec_in1.txt");
}

static void	setup_pipe_input(void)
{
	cleanup_all_files();
	int	fd = open("exec_in1.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);

	if (fd >= 0)
	{
		write(fd, "hello pipe\n", 11);
		close(fd);
	}
}

static int	assert_single_builtin_pwd(t_minishell *mini)
{
	int	fd = open("exec_out1.txt", O_RDONLY);

	if (fd < 0) return (0);
		char	buf[1024];

	ft_bzero(buf, sizeof(buf));
	ssize_t	bytes = read(fd, buf, sizeof(buf) - 1);

	close(fd);
	return (bytes > 0 && mini->exit_status == 0);
}

static int	assert_single_external_cmd(t_minishell *mini)
{
	int	fd = open("exec_out1.txt", O_RDONLY);

	if (fd < 0) return (0);
		char	buf[64];

	ft_bzero(buf, sizeof(buf));
	read(fd, buf, sizeof(buf) - 1);
	close(fd);
	return (ft_strncmp(buf, "hello exec_main\n", 16) == 0 && mini->exit_status == 0);
}

static int	assert_pipeline(t_minishell *mini)
{
	int	fd = open("exec_out1.txt", O_RDONLY);

	if (fd < 0) return (0);
		char	buf[64];

	ft_bzero(buf, sizeof(buf));
	read(fd, buf, sizeof(buf) - 1);
	close(fd);
	return (ft_strncmp(buf, "hello pipe\n", 11) == 0 && mini->exit_status == 0);
}

static int	assert_multi_pipe_wc(t_minishell *mini)
{
	int	fd = open("exec_out1.txt", O_RDONLY);

	if (fd < 0) return (0);
		char	buf[64];

	ft_bzero(buf, sizeof(buf));
	read(fd, buf, sizeof(buf) - 1);
	close(fd);
	/* "hello pipe\n" filtered through grep "hello" produces 1 line */
	return (ft_atoi(buf) == 1 && mini->exit_status == 0);
}

static int	assert_multi_pipe_rev(t_minishell *mini)
{
	int	fd = open("exec_out1.txt", O_RDONLY);

	if (fd < 0) return (0);
		char	buf[64];

	ft_bzero(buf, sizeof(buf));
	read(fd, buf, sizeof(buf) - 1);
	close(fd);
	/* "hello pipe\n" -> grep "hello" -> tr lower-to-upper -> "HELLO PIPE\n" -> rev -> "EPIP OLLEH\n" */
	return (ft_strncmp(buf, "EPIP OLLEH\n", 11) == 0 && mini->exit_status == 0);
}

static int	assert_mid_cmd_failure(t_minishell *mini)
{
	int	fd = open("exec_out1.txt", O_RDONLY);

	if (fd < 0) return (0);
		char	buf[64];

	ft_bzero(buf, sizeof(buf));
	read(fd, buf, sizeof(buf) - 1);
	close(fd);
	return (ft_strncmp(buf, "pipeline finished\n", 18) == 0 && mini->exit_status == 0);
}

/* --- RUNNER ENGINE --- */

static int	run_single_exec_test(t_exec_test *test, t_single_linked_node *envp)
{
	pid_t	pid;
	int		pipe_fd[2];

	if (test->setup_files)
		test->setup_files();
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
		t_minishell	mini;

		init_mock_minishell(&mini);
		/* Clone list into mini so exec_main operates on isolated memory */
		t_single_linked_node	*cloned_cmds = clone_cmd_list(test->cmd_lst);

		mini.cmd_lst = cloned_cmds;
		printf("----------------------------------------------------------------------------------------------------------\n");
		exec_main(&mini, mini.cmd_lst, &envp);
		write(pipe_fd[1], &mini.exit_status, sizeof(int));
		int	assert_res = 1;

		if (test->assert_behavior)
			assert_res = test->assert_behavior(&mini);
		write(pipe_fd[1], &assert_res, sizeof(int));
		clear_cmd_list(&cloned_cmds);
		clear_cmd_list(&test->cmd_lst);
		ft_single_lstclear(&envp, del_env_node_content);
		close(pipe_fd[1]);
		exit(0);
	}
	close(pipe_fd[1]);
	int	status;

	waitpid(pid, &status, 0);
	int	actual_exit_status = -1;
	int	behavior_pass = 0;

	fcntl(pipe_fd[0], F_SETFL, O_NONBLOCK);
	read(pipe_fd[0], &actual_exit_status, sizeof(int));
	read(pipe_fd[0], &behavior_pass, sizeof(int));
	close(pipe_fd[0]);
	int	pass = 1;

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
	return (pass);
}

/* --- MAIN TEST SUITE INTERFACE --- */

int	test_general_cmd_exec(void)
{
	int	success = 1;

	printf("\n--- Running exec_main Execution Engine Suite ---\n");
	extern char				**environ;
	t_single_linked_node	*envp = env_to_lst(environ);
	/* --------------------------------------------------------------------- */
	/* Test Case 1: Builtin Redirection (pwd > exec_out1.txt)                */
	/* --------------------------------------------------------------------- */

	{
		static char	*pwd_argv[] =
		{
			"pwd", NULL
		};
		t_single_linked_node	*r1 = create_redir_node("exec_out1.txt", OUT);

		t_exec_test	t1 =
		{
			.test_name = "1. Single Builtin with Redirection (pwd > file)",
			.cmd_lst = create_cmd_node(pwd_argv, r1),
			.expected_exit_status = 0,
			.setup_files = cleanup_out_files,
			.cleanup_files = cleanup_out_files,
			.assert_behavior = assert_single_builtin_pwd
		};
		if (!run_single_exec_test(&t1, envp))
			success = 0;
		clear_cmd_list(&t1.cmd_lst);
	}
	/* --------------------------------------------------------------------- */
	/* Test Case 2: External Command (echo text > exec_out1.txt)            */
	/* --------------------------------------------------------------------- */
	{
		static char	*echo_argv[] =
		{
			"echo", "hello exec_main", NULL
		};
		t_single_linked_node	*r2 = create_redir_node("exec_out1.txt", OUT);

		t_exec_test	t2 =
		{
			.test_name = "2. Single External Command (echo text > file)",
			.cmd_lst = create_cmd_node(echo_argv, r2),
			.expected_exit_status = 0,
			.setup_files = cleanup_out_files,
			.cleanup_files = cleanup_out_files,
			.assert_behavior = assert_single_external_cmd
		};
		if (!run_single_exec_test(&t2, envp))
			success = 0;
		clear_cmd_list(&t2.cmd_lst);
	}
	/* --------------------------------------------------------------------- */
	/* Test Case 3: Pure Pipeline (cat exec_in1.txt | grep hello > out)      */
	/* --------------------------------------------------------------------- */
	{
		static char	*cat_argv3[] =
		{
			"cat", "exec_in1.txt", NULL
		};
		static char	*grep_argv3[] =
		{
			"grep", "hello", NULL
		};
		t_single_linked_node	*r3_out = create_redir_node("exec_out1.txt", OUT);
		t_single_linked_node	*cmd3_1 = create_cmd_node(cat_argv3, NULL);
		t_single_linked_node	*cmd3_2 = create_cmd_node(grep_argv3, r3_out);

		cmd3_1->next = cmd3_2;
		t_exec_test	t3 =
		{
			.test_name = "3. Pure Pipeline (cat file | grep > file)",
			.cmd_lst = cmd3_1,
			.expected_exit_status = 0,
			.setup_files = setup_pipe_input,
			.cleanup_files = cleanup_all_files,
			.assert_behavior = assert_pipeline
		};
		if (!run_single_exec_test(&t3, envp))
			success = 0;
		clear_cmd_list(&t3.cmd_lst);
	}
	/* --------------------------------------------------------------------- */
	/* Test Case 4: Pipeline + Redirection (cat < exec_in1.txt | grep > out)  */
	/* --------------------------------------------------------------------- */
	{
		static char	*cat_no_args[] =
		{
			"cat", NULL
		};
		static char	*grep_argv4[] =
		{
			"grep", "hello", NULL
		};
		t_single_linked_node	*r4_in = create_redir_node("exec_in1.txt", IN);
		t_single_linked_node	*r4_out = create_redir_node("exec_out1.txt", OUT);
		t_single_linked_node	*cmd4_1 = create_cmd_node(cat_no_args, r4_in);
		t_single_linked_node	*cmd4_2 = create_cmd_node(grep_argv4, r4_out);

		cmd4_1->next = cmd4_2;
		t_exec_test	t4 =
		{
			.test_name = "4. Pipeline + Input Redirection (cat < file | grep > file)",
			.cmd_lst = cmd4_1,
			.expected_exit_status = 0,
			.setup_files = setup_pipe_input,
			.cleanup_files = cleanup_all_files,
			.assert_behavior = assert_pipeline
		};
		if (!run_single_exec_test(&t4, envp))
			success = 0;
		clear_cmd_list(&t4.cmd_lst);
	}
	/* --------------------------------------------------------------------- */
	/* Test Case 5: Invalid Command Handling                                 */
	/* --------------------------------------------------------------------- */
	{
		static char	*invalid_argv[] =
		{
			"non_existent_command_12345", NULL
		};
		t_exec_test	t5 =
		{
			.test_name = "5. Invalid Command Handling (Return 127)",
			.cmd_lst = create_cmd_node(invalid_argv, NULL),
			.expected_exit_status = 127,
			.setup_files = cleanup_all_files,
			.cleanup_files = cleanup_all_files
		};
		if (!run_single_exec_test(&t5, envp))
			success = 0;
		clear_cmd_list(&t5.cmd_lst);
	}
	/* --------------------------------------------------------------------- */
	/* Test Case 6: Multi-Pipe Chain (cat | grep | wc -l > file)            */
	/* --------------------------------------------------------------------- */
	{
		static char	*cat_argv6[] =
		{
			"cat", "exec_in1.txt", NULL
		};
		static char	*grep_argv6[] =
		{
			"grep", "hello", NULL
		};
		static char	*wc_argv6[] =
		{
			"wc", "-l", NULL
		};
		t_single_linked_node	*r6_out = create_redir_node("exec_out1.txt", OUT);
		t_single_linked_node	*cmd6_1 = create_cmd_node(cat_argv6, NULL);
		t_single_linked_node	*cmd6_2 = create_cmd_node(grep_argv6, NULL);
		t_single_linked_node	*cmd6_3 = create_cmd_node(wc_argv6, r6_out);

		cmd6_1->next = cmd6_2;
		cmd6_2->next = cmd6_3;
		t_exec_test	t6 =
		{
			.test_name = "6. Multi-Pipe Chain (cat file | grep hello | wc -l > out)",
			.cmd_lst = cmd6_1,
			.expected_exit_status = 0,
			.setup_files = setup_pipe_input,
			.cleanup_files = cleanup_all_files,
			.assert_behavior = assert_multi_pipe_wc
		};
		if (!run_single_exec_test(&t6, envp))
			success = 0;
		clear_cmd_list(&t6.cmd_lst);
	}
	/* --------------------------------------------------------------------- */
	/* Test Case 7: 4-Stage Pipeline with Input/Output Redirection          */
	/* (cat < in | grep hello | tr a-z A-Z | rev > out)                     */
	/* --------------------------------------------------------------------- */
	{
		static char	*cat_argv7[] =
		{
			"cat", NULL
		};
		static char	*grep_argv7[] =
		{
			"grep", "hello", NULL
		};
		static char	*tr_argv7[] =
		{
			"tr", "a-z", "A-Z", NULL
		};
		static char	*rev_argv7[] =
		{
			"rev", NULL
		};
		t_single_linked_node	*r7_in = create_redir_node("exec_in1.txt", IN);
		t_single_linked_node	*r7_out = create_redir_node("exec_out1.txt", OUT);
		t_single_linked_node	*cmd7_1 = create_cmd_node(cat_argv7, r7_in);
		t_single_linked_node	*cmd7_2 = create_cmd_node(grep_argv7, NULL);
		t_single_linked_node	*cmd7_3 = create_cmd_node(tr_argv7, NULL);
		t_single_linked_node	*cmd7_4 = create_cmd_node(rev_argv7, r7_out);

		cmd7_1->next = cmd7_2;
		cmd7_2->next = cmd7_3;
		cmd7_3->next = cmd7_4;
		t_exec_test	t7 =
		{
			.test_name = "7. 4-Stage Pipe (cat < in | grep | tr | rev > out)",
			.cmd_lst = cmd7_1,
			.expected_exit_status = 0,
			.setup_files = setup_pipe_input,
			.cleanup_files = cleanup_all_files,
			.assert_behavior = assert_multi_pipe_rev
		};
		if (!run_single_exec_test(&t7, envp))
			success = 0;
		clear_cmd_list(&t7.cmd_lst);
	}
	/* --------------------------------------------------------------------- */
	/* Test Case 8: Multi-Pipe with Mid-Pipeline Failure Exit Status        */
	/* (cat in | invalid_cmd | grep hello) -> expected status 0             */
	/* --------------------------------------------------------------------- */
	{
		static char	*cat_argv8[] =
		{
			"cat", "exec_in1.txt", NULL
		};
		static char	*invalid_argv8[] =
		{
			"non_existent_command_12345", NULL
		};
		static char	*grep_argv8[] =
		{
			"grep", "hello", NULL
		};
		t_single_linked_node	*cmd8_1 = create_cmd_node(cat_argv8, NULL);
		t_single_linked_node	*cmd8_2 = create_cmd_node(invalid_argv8, NULL);
		t_single_linked_node	*cmd8_3 = create_cmd_node(grep_argv8, NULL);

		cmd8_1->next = cmd8_2;
		cmd8_2->next = cmd8_3;
		t_exec_test	t8 =
		{
			.test_name = "8. Multi-Pipe Failure propagation (cat | invalid_cmd | grep)",
			.cmd_lst = cmd8_1,
			.expected_exit_status = 1,
			.setup_files = setup_pipe_input,
			.cleanup_files = cleanup_all_files,
			.assert_behavior = NULL
		};
		if (!run_single_exec_test(&t8, envp))
			success = 0;
		clear_cmd_list(&t8.cmd_lst);
	}
	/* --------------------------------------------------------------------- */
	/* Test Case 9: Middle Command Failure (cmd1 | invalid_cmd | cmd3)        */
	/* Pipeline status must reflect the LAST command (cmd3 = 0).            */
	/* --------------------------------------------------------------------- */
	{
		static char	*cat_argv9[] =
		{
			"cat", "exec_in1.txt", NULL
		};
		static char	*invalid_argv9[] =
		{
			"non_existent_command_999", NULL
		};
		static char	*echo_argv9[] =
		{
			"echo", "pipeline finished", NULL
		};
		t_single_linked_node	*r9_out = create_redir_node("exec_out1.txt", OUT);
		t_single_linked_node	*cmd9_1 = create_cmd_node(cat_argv9, NULL);
		t_single_linked_node	*cmd9_2 = create_cmd_node(invalid_argv9, NULL);
		t_single_linked_node	*cmd9_3 = create_cmd_node(echo_argv9, r9_out);

		cmd9_1->next = cmd9_2;
		cmd9_2->next = cmd9_3;
		t_exec_test	t9 =
		{
			.test_name = "9. Middle Command Failure (cat | invalid_cmd | echo > out)",
			.cmd_lst = cmd9_1,
			.expected_exit_status = 0,
			.setup_files = setup_pipe_input,
			.cleanup_files = cleanup_all_files,
			.assert_behavior = assert_mid_cmd_failure
		};
		if (!run_single_exec_test(&t9, envp))
			success = 0;
		clear_cmd_list(&t9.cmd_lst);
	}
	ft_single_lstclear(&envp, del_env_node_content);
	return (success);
}
