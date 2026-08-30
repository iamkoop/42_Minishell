/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manipulation_tests.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 19:10:17 by username          #+#    #+#             */
/*   Updated: 2026/08/30 14:52:55 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "testing.h"

typedef struct s_get_env_test
{
	char	*test_name;
	char	*to_find;
	char	*expected_value;
}	t_get_env_test;

static void	free_char_matrix(char **matrix)
{
	int	i;

	if (!matrix)
		return ;
	i = 0;
	while (matrix[i])
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
}

// --- Target 3 Validation: Runs isolated tests for get_env_from_lst ---

static int	test_get_env_func(t_single_linked_node *lst)
{
	int	success = 1;

	t_get_env_test	tests[] =
	{
		{
			"Find standard existing key", "VALID", "123"
		},
		{
			"Find key with internal assignment tokens", "PATH", "/usr/bin=/bin"
		},
		{
			"Find key with empty value token", "EMPTY_VAL", ""
		},
		{
			"Look up non-existent key", "NOT_FOUND_KEY", NULL
		},
		{
			"Look up partial key match (security check)", "VALI", NULL
		}
	};
	int	num_tests = sizeof(tests) / sizeof(tests[0]);

	printf("\n--- Running get_env_from_lst Tests ---\n");
	// Guard edgecase: Querying a completely empty list
	t_single_linked_node	*node = get_env_from_lst("VALID", NULL);

	if (node != NULL)
	{
		printf("❌ FAIL: get_env_from_lst on NULL list returned a node!\n");
		success = 0;
	}
	else
		printf("✅ PASS: Guard check on NULL list reference\n");
	for (int i = 0; i < num_tests; i++)
	{
		node = get_env_from_lst(tests[i].to_find, lst);
		if (tests[i].expected_value == NULL)
		{
			if (node == NULL || ((t_env_var *) node->content)->value == NULL)
				printf("✅ PASS: %s\n", tests[i].test_name);
			else
			{
				printf("❌ FAIL: %s\n", tests[i].test_name);
				printf("   Expected: NULL node pointer or NULL value\n");
				printf("   Got Key : \"%s\" with Value: \"%s\"\n",
					((t_env_var *) node->content)->key, ((t_env_var *) node->content)->value);
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
				continue ;
			}
			t_env_var	*var = (t_env_var *) node->content;
			char		*actual_val = var->value ? var->value : "";

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

// --- Target 1 Helper Validation: Matrix to Linked List Integrity Verification ---

static int	verify_matrix_to_lst(char **envp, t_single_linked_node *lst)
{
	int						i = 0;
	t_single_linked_node	*curr = lst;

	if ((!envp || !*envp) && !lst)
		return (1);
	if ((!envp || !*envp) || !lst)
		return (0);
	while (envp[i] != NULL && curr != NULL)
	{
		t_env_var	*var = (t_env_var *) curr->content;

		if (!var || !var->key)
			return (0);
		size_t	key_len = strlen(var->key);
		char	*has_equal = strchr(envp[i], '=');

		if (!has_equal)
		{
			if (strcmp(envp[i], var->key) != 0)
				return (0);
		}
		else if (strncmp(envp[i], var->key, key_len) != 0 || envp[i][key_len] != '=')
			return (0);
		else
		{
			char	*expected_val = envp[i] + key_len + 1;
			char	*actual_val = var->value ? var->value : "";

			if (strcmp(expected_val, actual_val) != 0)
				return (0);
		}
		curr = curr->next;
		i++;
	}
	return (envp[i] == NULL && curr == NULL);
}

// --- Target 2 Helper Validation: Linked List to Matrix Integrity Verification ---

static int	verify_lst_to_matrix(t_single_linked_node *lst, char **matrix)
{
	int						i = 0;
	t_single_linked_node	*curr = lst;

	if (!lst && (!matrix || !*matrix))
		return (1);
	if (!lst || !matrix)
		return (0);
	while (curr != NULL && matrix[i] != NULL)
	{
		t_env_var	*var = (t_env_var *) curr->content;

		if (!var || !var->key)
			return (0);
		char	*actual_val = var->value ? var->value : "";
		size_t	alloc_size = strlen(var->key) + strlen(actual_val) + 2;
		char	*expected_str = malloc(alloc_size);

		if (!expected_str)
			return (0);
		if (!var->value || strlen(var->value) == 0)
			sprintf(expected_str, "%s", var->key);
		else
			sprintf(expected_str, "%s=%s", var->key, actual_val);
		int	match = (strcmp(expected_str, matrix[i]) == 0);

		free(expected_str);
		if (!match)
			return (0);
		curr = curr->next;
		i++;
	}
	return (curr == NULL && matrix[i] == NULL);
}

// --- Pipeline Testing Framework Suite Engine ---

int	env_manipulation_tests(char **envp)
{
	int	success = 1;

	char	*case1[] =
	{
		NULL
	};
	char	*case2[] =
	{
		"EMPTY_VAL=", "VALID=123", NULL
	};
	// Fixed lookup test dependency array:
	char	*case3[] =
	{
		"PATH=/usr/bin=/bin", "EQUALS======", "VALID=123", "EMPTY_VAL=", NULL
	};
	char	*case4[] =
	{
		"SPECIAL=~!@#$%^&*()_+{}|:<>?-=[]\\;',./", "VALID=123", NULL
	};
	struct
	{
		char	*name;
		char	**env_matrix;

	} conversion_cases[] =
	{
		{
			"Empty Environment Matrix (env -i)", case1
		},
		{
			"Keys explicitly ending with '='", case2
		},
		{
			"Values loaded with internal '=' assignments", case3
		},
		{
			"Raw special/symbolic payload values", case4
		},
	};
	int	num_cases = sizeof(conversion_cases) / sizeof(conversion_cases[0]);

	printf("\n--- Running Env Conversion Pipeline Tests (env_to_lst & env_to_char_arr) ---\n");
	for (int i = 0; i < num_cases; i++)
	{
		t_single_linked_node	*lst = env_to_lst(conversion_cases[i].env_matrix);
		char					**reverted_matrix = env_to_char_arr(lst);
		int						dir1 = verify_matrix_to_lst(conversion_cases[i].env_matrix, lst);
		int						dir2 = verify_lst_to_matrix(lst, reverted_matrix);

		if (dir1 && dir2)
			printf("✅ PASS: %s\n", conversion_cases[i].name);
		else
		{
			printf("❌ FAIL: %s\n", conversion_cases[i].name);
			if (!dir1) printf("   -> env_to_lst Conversion Failure\n");
				if (!dir2) printf("   -> env_to_char_arr Conversion Failure\n");
				success = 0;
		}
		// Guarantees allocation teardown under any pass/fail scenario
		ft_single_lstclear(&lst, del_env_node_content);
		free_char_matrix(reverted_matrix);
	}
	// --- Dynamic Host OS Validation Check ---
	t_single_linked_node	*host_lst = env_to_lst(envp);
	char					**host_reverted = env_to_char_arr(host_lst);

	if (verify_matrix_to_lst(envp, host_lst) && verify_lst_to_matrix(host_lst, host_reverted))
		printf("✅ PASS: Standard Host System Environment Matrix Parsing\n");
	else
	{
		printf("❌ FAIL: Standard Host System Environment Matrix Parsing\n");
		success = 0;
	}
	free_char_matrix(host_reverted);
	// --- Testing target 3: get_env_from_lst lookup suite ---
	t_single_linked_node	*lookup_lst = env_to_lst(case3);

	if (!test_get_env_func(lookup_lst))
		success = 0;
	ft_single_lstclear(&lookup_lst, del_env_node_content);
	ft_single_lstclear(&host_lst, del_env_node_content);
	return (success);
}
