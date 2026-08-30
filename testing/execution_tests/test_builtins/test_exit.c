/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 17:48:48 by username          #+#    #+#             */
/*   Updated: 2026/08/30 17:18:35 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "testing.h"

typedef struct s_exit_test
{
	char	*test_name;
	char	**input;
	int		expected_status;
	char	*expected_stdout;
	// Checked via strstr (usually "exit")
	char	*expected_err_msg;
	// Checked via strstr (NULL if none)
}	t_exit_test;

static int	run_single_exit_test(t_exit_test test)
{
	int		out_pipe[2];
	int		err_pipe[2];
	pid_t	pid;

	if (pipe(out_pipe) == -1 || pipe(err_pipe) == -1)
	{
		perror("pipe failed");
		return (0);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		close(out_pipe[0]);
		close(out_pipe[1]);
		close(err_pipe[0]);
		close(err_pipe[1]);
		return (0);
	}
	if (pid == 0)
	{
		// Child Process Sandbox: Route stdout & stderr out through the pipes
		close(out_pipe[0]);
		if (dup2(out_pipe[1], STDOUT_FILENO) == -1)
			exit(1);
		close(out_pipe[1]);
		close(err_pipe[0]);
		if (dup2(err_pipe[1], STDERR_FILENO) == -1)
			exit(1);
		close(err_pipe[1]);
		// ==========================================
		// CALLING YOUR EXIT IMPLEMENTATION
		// ==========================================
		builtin_exit(test.input);
		// ==========================================
		exit(1);
	}
	// Parent Process
	close(out_pipe[1]);
	close(err_pipe[1]);
	int	status;

	waitpid(pid, &status, 0);
	// Capture stdout data
	char	out_buffer[4096];

	memset(out_buffer, 0, sizeof(out_buffer));
	fcntl(out_pipe[0], F_SETFL, O_NONBLOCK);
	read(out_pipe[0], out_buffer, sizeof(out_buffer) - 1);
	close(out_pipe[0]);
	// Capture stderr data
	char	err_buffer[4096];

	memset(err_buffer, 0, sizeof(err_buffer));
	fcntl(err_pipe[0], F_SETFL, O_NONBLOCK);
	read(err_pipe[0], err_buffer, sizeof(err_buffer) - 1);
	close(err_pipe[0]);
	int	actual_status = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
	int	pass = 1;

	// 1. Verify Exit Code
	if (actual_status != test.expected_status)
	{
		printf("❌ FAIL: %s [Status Mismatch]\n", test.test_name);
		printf("   Expected Exit Status: %d, Got: %d\n", test.expected_status, actual_status);
		pass = 0;
	}
	// 2. Verify Output Text (Stdout)
	if (test.expected_stdout && strstr(out_buffer, test.expected_stdout) == NULL)
	{
		if (pass) printf("❌ FAIL: %s [STDOUT Mismatch]\n", test.test_name);
			printf("   Expected stdout to contain: \"%s\"\n", test.expected_stdout);
		printf("   Got stdout                : \"%s\"\n", out_buffer);
		pass = 0;
	}
	// 3. Verify Error Messages (Stderr)
	if (test.expected_err_msg)
	{
		if (strstr(err_buffer, test.expected_err_msg) == NULL)
		{
			if (pass) printf("❌ FAIL: %s [STDERR Mismatch]\n", test.test_name);
				printf("   Expected Error Msg to contain: \"%s\"\n", test.expected_err_msg);
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

int	test_builtin_exit(char **envp)
{
	(void) envp;
	int	success = 1;

	// Fixed string element structures — 0 dynamic allocation overhead
	char	*case1[] =
	{
		"exit", NULL
	};
	char	*case2[] =
	{
		"exit", "0", NULL
	};
	char	*case3[] =
	{
		"exit", "42", NULL
	};
	char	*case4[] =
	{
		"exit", "259", NULL
	};
	// Overflow: 259 % 256 = 3
	char	*case5[] =
	{
		"exit", "-5", NULL
	};
	// Underflow: 256 - 5 = 251
	char	*case6[] =
	{
		"exit", "9223372036854775807", NULL
	};
	// LLONG_MAX -> Status 255
	char	*case7[] =
	{
		"exit", "9223372036854775808", NULL
	};
	// Out of range signed bounds -> Bash Status 2
	char	*case8[] =
	{
		"exit", "not_a_num", NULL
	};
	// Numeric argument check -> Bash Status 2
	char	*case9[] =
	{
		"exit", "1", "2", NULL
	};
	// Multi-argument check
	t_exit_test	tests[] =
	{
		{
			"Plain exit (Default status 0)", case1, 0, "exit", NULL
		},
		{
			"Exit with zero status input", case2, 0, "exit", NULL
		},
		{
			"Exit with positive status code 42", case3, 42, "exit", NULL
		},
		{
			"Exit with overflow value (259)", case4, 3, "exit", NULL
		},
		{
			"Exit with negative value (-5)", case5, 251, "exit", NULL
		},
		{
			"Exit with maximum long long limit", case6, 255, "exit", NULL
		},
		{
			"Exit value exceeding long long bounds", case7, 2, "exit", "numeric argument required"
		},
		{
			"Non-numeric structural argument string", case8, 2, "exit", "numeric argument required"
		},
		{
			"Too many positional arguments parsed", case9, 1, "exit", "too many arguments"
		}
	};
	int	num_tests = sizeof(tests) / sizeof(tests[0]);

	printf("\n--- Running Exit Builtin Tests ---\n");
	for (int i = 0; i < num_tests; i++)
	{
		if (!run_single_exit_test(tests[i]))
			success = 0;
	}
	return (success);
}
