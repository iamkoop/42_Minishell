/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* env_test.c                                         :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2026/05/19 17:26:57 by nildruon          #+#    #+#             */
/* Updated: 2026/05/26 18:30:00 by nildruon         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "testing.h"

static void free_char_matrix(char **matrix)
{
	int i;

	if (!matrix)
		return;
	i = 0;
	while (matrix[i])
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
}

/* ========================================================================== */
/* 1. TEST DIRECTION: envp[][] -> Linked List                                 */
/* ========================================================================== */
static int test_creation(char **envp, t_single_linked_node *curr_node)
{
	int i = 0;
	int tests_failed = 0;

	// Edge Case: Completely Empty environment
	if (!envp || !*envp)
	{
		if (!curr_node)
			printf("  [OK]   Direction 1: Empty environment conversion handled correctly.\n");
		else
		{
			printf("  [FAIL] Direction 1: Empty environment produced a non-empty Linked List.\n");
			tests_failed++;
		}
		return (tests_failed == 0);
	}

	while (envp[i] != NULL && curr_node != NULL)
	{
		t_env_var *var = (t_env_var *)curr_node->content;
		
		if (!var || !var->key)
		{
			printf("  [FAIL] Direction 1 (Index %d): Node content or key is NULL.\n", i);
			tests_failed++;
			curr_node = curr_node->next;
			i++;
			continue;
		}

		size_t key_len = strlen(var->key);
		
		// Handle Edge Case: String without an '=' sign (e.g. export KEY)
		char *has_equal = strchr(envp[i], '=');
		if (!has_equal)
		{
			if (strcmp(envp[i], var->key) == 0 && (var->value == NULL || var->value[0] == '\0'))
				printf("  [OK]   Direction 1 (Index %d): Valueless key '%s' parsed correctly.\n", i, var->key);
			else
			{
				printf("  [FAIL] Direction 1 (Index %d): Mismatch on valueless key.\n", i);
				printf("         Expected Key: %s, Got: %s\n", envp[i], var->key);
				tests_failed++;
			}
		}
		// Standard verification for KEY=VALUE
		else if (strncmp(envp[i], var->key, key_len) != 0 || envp[i][key_len] != '=')
		{
			printf("  [FAIL] Direction 1 (Index %d): Key mismatch.\n", i);
			printf("         Expected: %s=...\n", var->key);
			printf("         Actual:   %s\n", envp[i]);
			tests_failed++;
		}
		else
		{
			char *expected_value = envp[i] + key_len + 1;
			char *actual_value = var->value ? var->value : "";

			if (strcmp(expected_value, actual_value) != 0)
			{
				printf("  [FAIL] Direction 1: Value mismatch for key '%s'.\n", var->key);
				printf("         Expected: \"%s\"\n", expected_value);
				printf("         Actual:   \"%s\"\n", actual_value);
				tests_failed++;
			}
		}
		curr_node = curr_node->next;
		i++;
	}

	if (envp[i] != NULL || curr_node != NULL)
	{
		printf("  [FAIL] Direction 1: Size mismatch between envp matrix and list nodes.\n");
		tests_failed++;
	}

	if (tests_failed == 0)
		printf("  [OK]   Direction 1: Matrix to Linked List conversion matching perfectly.\n");
	return (tests_failed == 0);
}

/* ========================================================================== */
/* 2. TEST DIRECTION: Linked List -> envp[][]                                 */
/* ========================================================================== */
static int test_reversion(t_single_linked_node *head, char **reverted_arr)
{
	int i = 0;
	int tests_failed = 0;
	t_single_linked_node *curr = head;

	if (!head)
	{
		if (!reverted_arr || !*reverted_arr)
			printf("  [OK]   Direction 2: Empty Linked List conversion handled correctly.\n");
		else
		{
			printf("  [FAIL] Direction 2: Empty Linked List produced a non-empty matrix.\n");
			tests_failed++;
		}
		return (tests_failed == 0);
	}

	if (!reverted_arr)
	{
		printf("  [FAIL] Direction 2: Reverted matrix pointer is completely NULL.\n");
		return (0);
	}

	while (curr != NULL && reverted_arr[i] != NULL)
	{
		t_env_var *var = (t_env_var *)curr->content;
		if (!var || !var->key)
		{
			curr = curr->next;
			i++;
			continue;
		}

		char *actual_value = var->value ? var->value : "";
		size_t allocation_size = strlen(var->key) + strlen(actual_value) + 2;
		char *expected_str = malloc(allocation_size);
		
		if (!expected_str)
		{
			printf("  [CRIT] Malloc error while preparing expected evaluation string.\n");
			return (0);
		}
		
		// If node value explicitly doesn't exist, check how your project exports it
		if (!var->value)
			sprintf(expected_str, "%s", var->key); // normal behavior for valueless keys
		else
			sprintf(expected_str, "%s=%s", var->key, actual_value);

		if (strcmp(expected_str, reverted_arr[i]) != 0)
		{
			printf("  [FAIL] Direction 2 (Index %d): String format error.\n", i);
			printf("         Expected output: %s\n", expected_str);
			printf("         Actual output:   %s\n", reverted_arr[i]);
			tests_failed++;
		}

		free(expected_str);
		curr = curr->next;
		i++;
	}

	if (reverted_arr[i] != NULL || curr != NULL)
	{
		printf("  [FAIL] Direction 2: Size mismatch between list nodes and regenerated matrix.\n");
		tests_failed++;
	}

	if (tests_failed == 0)
		printf("  [OK]   Direction 2: Linked List to Matrix reconstruction matching perfectly.\n");
	return (tests_failed == 0);
}

/* ========================================================================== */
/* TEST EXECUTION HARNESS                                                     */
/* ========================================================================== */
static int run_test_suite(const char *test_name, char **test_envp)
{
	printf("🔷 [SUITE] %s\n", test_name);

	t_single_linked_node *list = env_to_lst(test_envp);
	if (!list && test_envp && *test_envp)
	{
		printf("  [FAIL] env_to_lst returned NULL (Premature allocation failure).\n\n");
		return (0);
	}

	int creation_ok = test_creation(test_envp, list);
	
	char **reverted_arr = env_to_char_arr(list);
	if (!reverted_arr && list)
	{
		printf("  [FAIL] env_to_char_arr returned NULL (Premature allocation failure).\n");
		ft_single_lstclear(&list, del_env_node_content);
		printf("\n");
		return (0);
	}

	int reversion_ok = test_reversion(list, reverted_arr);

	ft_single_lstclear(&list, del_env_node_content);
	free_char_matrix(reverted_arr);
	printf("\n");

	return (creation_ok && reversion_ok);
}

/* ========================================================================== */
/* MAIN TEST CONTROLLER                                                       */
/* ========================================================================== */
int env_tests(char **envp)
{
	int global_success = 1;

	printf("====================================================\n");
	printf("       RUNNING MINI-SHELL ENVIRONMENT TESTER        \n");
	printf("====================================================\n\n");

	// --- EDGE CASE 1: Completely Empty Environment Matrix ---
	char *empty_env[] = { NULL };
	global_success &= run_test_suite("Edge Case: Completely Empty Environment (env -i)", empty_env);

	// --- EDGE CASE 2: Key with completely empty value ---
	char *empty_val_env[] = { "EMPTY_VAL=", "NORMAL=abc", NULL };
	global_success &= run_test_suite("Edge Case: Keys ending explicitly with '='", empty_val_env);

	// --- EDGE CASE 3: Keys with no '=' symbol (Exported variables) ---
	char *no_equal_env[] = { "NO_EQUAL_SIGN", "VALID=123", NULL };
	global_success &= run_test_suite("Edge Case: Valueless identifier attributes", no_equal_env);

	// --- EDGE CASE 4: Values containing internal '=' characters ---
	char *multi_equal_env[] = { "PATH=/usr/bin=/bin", "EQUALS======", NULL };
	global_success &= run_test_suite("Edge Case: Values loaded with internal assignment tokens", multi_equal_env);

	// --- EDGE CASE 5: Odd alphanumeric characters inside variable values ---
	char *special_chars_env[] = { "SPECIAL=~!@#$%^&*()_+{}|:<>?-=[]\\;',./", NULL };
	global_success &= run_test_suite("Edge Case: Raw special/symbolic value payloads", special_chars_env);

	// --- TEST 6: Actual Host System Environment ---
	global_success &= run_test_suite("Standard Suite: Host System Environment Matrix", envp);

	printf("----------------------------------------------------\n");
	if (global_success)
		printf("🎉 VERDICT: ALL TEST SUITES PASSED VALIDATION!\n");
	else
		printf("❌ VERDICT: ONE OR MORE EDGE CASES ENCOUNTERED REJECTIONS.\n");
	printf("====================================================\n\n");

	return (!global_success);
}