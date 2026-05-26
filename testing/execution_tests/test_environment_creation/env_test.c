/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* env_test.c                                         :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2026/05/19 17:26:57 by nildruon          #+#    #+#             */
/* Updated: 2026/05/26 18:00:00 by nildruon         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "testing.h"

// Helper to free a null-terminated array of strings (matrix)
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

	printf("🔸 [TEST] Direction 1: envp[][] -> Linked List\n");

	// Edge Case: Empty environment
	if (!envp || !*envp)
	{
		if (!curr_node)
			printf("  [OK]   Empty envp successfully produced an empty Linked List.\n");
		else
		{
			printf("  [FAIL] Empty envp produced a non-empty Linked List.\n");
			tests_failed++;
		}
		return (tests_failed == 0);
	}

	while (envp[i] != NULL && curr_node != NULL)
	{
		t_env_var *var = (t_env_var *)curr_node->content;
		
		if (!var || !var->key)
		{
			printf("  [FAIL] Node %d: Node content or key is NULL\n", i);
			tests_failed++;
			curr_node = curr_node->next;
			i++;
			continue;
		}

		// Verify Key
		size_t key_len = strlen(var->key);
		if (strncmp(envp[i], var->key, key_len) != 0 || envp[i][key_len] != '=')
		{
			printf("  [FAIL] Node %d: Key mismatch.\n", i);
			printf("         Expected start: %s=\n", var->key);
			printf("         Actual envp:    %s\n", envp[i]);
			tests_failed++;
		}
		else
		{
			// Verify Value (Handles "KEY=" empty string cases)
			char *expected_value = envp[i] + key_len + 1;
			char *actual_value = var->value ? var->value : "";

			if (strcmp(expected_value, actual_value) != 0)
			{
				printf("  [FAIL] Value mismatch for key '%s'.\n", var->key);
				printf("         Expected: \"%s\"\n", expected_value);
				printf("         Actual:   \"%s\"\n", actual_value);
				tests_failed++;
			}
		}
		curr_node = curr_node->next;
		i++;
	}

	// Length Verification
	if (envp[i] != NULL)
	{
		printf("  [FAIL] Mismatch: envp has more items than the linked list.\n");
		tests_failed++;
	}
	else if (curr_node != NULL)
	{
		printf("  [FAIL] Mismatch: Linked list has more items than envp.\n");
		tests_failed++;
	}

	if (tests_failed == 0)
		printf("  [OK]   All nodes match envp perfectly.\n");

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

	printf("🔸 [TEST] Direction 2: Linked List -> envp[][]\n");

	// Edge Case: Empty list
	if (!head)
	{
		if (!reverted_arr || !*reverted_arr)
			printf("  [OK]   Empty Linked List successfully produced an empty array.\n");
		else
		{
			printf("  [FAIL] Empty Linked List produced a non-empty array.\n");
			tests_failed++;
		}
		return (tests_failed == 0);
	}

	if (!reverted_arr)
	{
		printf("  [FAIL] Reverted array pointer is completely NULL while list has items.\n");
		return (0);
	}

	while (curr != NULL && reverted_arr[i] != NULL)
	{
		t_env_var *var = (t_env_var *)curr->content;
		if (!var || !var->key)
		{
			curr = curr->next;
			i++;
			continue; // test_creation handles logging this node fault
		}

		// Reconstruct expected text format: "KEY=VALUE"
		char *actual_value = var->value ? var->value : "";
		size_t allocation_size = strlen(var->key) + strlen(actual_value) + 2;
		char *expected_str = malloc(allocation_size);
		
		if (!expected_str)
		{
			printf("  [CRIT] Malloc error during test string creation.\n");
			return (0);
		}
		sprintf(expected_str, "%s=%s", var->key, actual_value);

		// Compare
		if (strcmp(expected_str, reverted_arr[i]) != 0)
		{
			printf("  [FAIL] Index %d: Reverted string format error.\n", i);
			printf("         Expected: %s\n", expected_str);
			printf("         Actual:   %s\n", reverted_arr[i]);
			tests_failed++;
		}

		free(expected_str);
		curr = curr->next;
		i++;
	}

	// Length Verification
	if (reverted_arr[i] != NULL)
	{
		printf("  [FAIL] Mismatch: Reverted array has more elements than the linked list.\n");
		tests_failed++;
	}
	else if (curr != NULL)
	{
		printf("  [FAIL] Mismatch: Linked list has more elements than reverted array.\n");
		tests_failed++;
	}

	if (tests_failed == 0)
		printf("  [OK]   Reverted array matches the Linked List perfectly.\n");

	return (tests_failed == 0);
}

/* ========================================================================== */
/* MAIN TEST CONTROLLER                                                       */
/* ========================================================================== */
int env_tests(char **envp)
{
	printf("\n=============================================\n");
	printf("   RUNNING ENVIRONMENT CONVERSION TESTS      \n");
	printf("=============================================\n");

	// 1. Generate the working linked list
	t_single_linked_node *curr_node = env_to_lst(envp);
	
	// Edge case safeguard for Malloc Failure
	if (!curr_node && envp && *envp)
	{
		printf("[CRIT] env_to_lst returned NULL (Potential Malloc Failure).\n");
		printf("RESULT: FAILURE\n\n");
		return (1);
	}

	// 2. Test Direction 1: envp to Linked List
	int creation_ok = test_creation(envp, curr_node);
	printf("\n");

	// 3. Generate the working back-reversion array 
	char **reverted_arr = env_to_char_arr(curr_node);
	
	if (!reverted_arr && curr_node)
	{
		printf("[CRIT] env_to_char_arr returned NULL (Potential Malloc Failure).\n");
		ft_single_lstclear(&curr_node, del_env_node_content);
		printf("RESULT: FAILURE\n\n");
		return (1);
	}

	// 4. Test Direction 2: Linked List back to envp
	int reversion_ok = test_reversion(curr_node, reverted_arr);

	// Cleanup EVERYTHING safely
	ft_single_lstclear(&curr_node, del_env_node_content);
	free_char_matrix(reverted_arr);

	printf("\n---------------------------------------------\n");
	if (creation_ok && reversion_ok)
	{
		printf("🎉 RESULT: ALL ENVIRONMENT TESTS PASSED!\n");
		printf("---------------------------------------------\n\n");
		return (0);
	}
	else
	{
		printf("❌ RESULT: SOME TESTS FAILED.\n");
		printf("---------------------------------------------\n\n");
		return (1);
	}
}