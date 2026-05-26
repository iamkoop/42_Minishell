/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_test.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 17:26:57 by nildruon          #+#    #+#             */
/*   Updated: 2026/05/26 15:51:01 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "testing.h"

static int test_creation(char **envp)
{
    printf("\n--- Running Environment creation Test ---\n");
    
    // 1. Generate the linked list using your function
    t_single_linked_node *curr_node = env_to_lst(envp);
	t_single_linked_node *start = curr_node;
    int i = 0;
    int tests_failed = 0;

	if(!envp || !*envp)
		return(0);
    // 2. Iterate through both envp and the linked list simultaneously
    while (envp[i] != NULL && curr_node != NULL)
    {
        t_env_var *var = (t_env_var *)curr_node->content;
        
        // Safety check to ensure content isn't NULL
        if (!var || !var->key)
        {
            printf("[FAIL] Test %d: Node content or key is NULL\n", i);
            tests_failed++;
            curr_node = curr_node->next;
            i++;
            continue;
        }

        // 3. Verify the key matches the start of the envp string
        size_t key_len = strlen(var->key);
        if (strncmp(envp[i], var->key, key_len) != 0 || envp[i][key_len] != '=')
        {
            printf("[FAIL] Test %d: Key mismatch.\n", i);
            printf("       Expected start: %s=\n", var->key);
            printf("       Actual envp:    %s\n", envp[i]);
            tests_failed++;
        }
        else
        {
            // 4. Verify the value matches everything after the '=' sign
            // envp[i] + key_len + 1 moves the pointer past the key and the '='
            char *expected_value = envp[i] + key_len + 1;
            
            // Handle cases where value might be empty (e.g., "KEY=")
            char *actual_value = var->value ? var->value : "";

            if (strcmp(expected_value, actual_value) != 0)
            {
                printf("[FAIL] Test %d: Value mismatch for key '%s'.\n", i, var->key);
                printf("       Expected value: \"%s\"\n", expected_value);
                printf("       Actual value:   \"%s\"\n", actual_value);
                tests_failed++;
            }
            else
            {
                printf("[OK]   Test %d: %s=%s\n", i, var->key, actual_value);
            }
        }

        // Move to the next item
        curr_node = curr_node->next;
        i++;
    }

    // 5. Check for length mismatches between envp and the list
    if (envp[i] != NULL)
    {
        printf("[FAIL] Mismatch: envp has more items than the linked list.\n");
        tests_failed++;
    }
    else if (curr_node != NULL)
    {
        printf("[FAIL] Mismatch: Linked list has more items than envp.\n");
        tests_failed++;
    }

    printf("---------------------------------------------\n");
    if (tests_failed == 0)
        printf("RESULT: ALL TESTS PASSED SUCCESSFULLY!\n\n");
    else
		printf("RESULT: %d TEST(S) FAILED.\n\n", tests_failed);
    // Return 1 for success (0 failures), 0 for failure
	ft_single_lstclear(&start, del_env_node_content);
    return (tests_failed == 0);
}

int env_tests(char **envp)
{
	t_single_linked_node *curr_node = env_to_lst(envp);
    
    // IF MALLOC FAILS HERE, STOP IMMEDIATELY. DON'T RUN FURTHER TESTS.
    if (!curr_node && envp && *envp)
        return (1);

    printf("\n--- Running Enviroment Conversion Tests ---\n");
    
    // Stop regenerating the list inside test_creation! 
    // Pass 'curr_node' into it instead of making it call env_to_lst again.
    test_creation(envp); 
    
    printf("---------------------------------------------\n\n");
    char    **str = env_to_char_arr(curr_node);
    if(!str)
    {
        ft_single_lstclear(&curr_node, del_env_node_content);
        return(1); 
    }
    
    int r = 0;
    for(int i = 0; str[i] ; i++)
    {
        printf("%s\n", str[i]);
        r++;
    }
    printf("%d\n", r);
    
    // Clean up the char** array strings and the array wrapper here too!
    // (Assuming you have a function like ft_free_matrix)
    
    ft_single_lstclear(&curr_node, del_env_node_content);
    printf("---------------------------------------------\n\n");
    return(0);
}