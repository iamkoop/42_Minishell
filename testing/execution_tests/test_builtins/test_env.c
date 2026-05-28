/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:50:10 by nildruon          #+#    #+#             */
/*   Updated: 2026/05/27 16:30:40 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "testing.h"

// --- Internal Helper: Verifies captured output string against expected envp array ---
static int verify_output(const char *buffer, char **envp)
{
    char *buffer_copy = strdup(buffer);
    if (!buffer_copy)
    {
        perror("strdup failed");
        return (0);
    }

    char *line = strtok(buffer_copy, "\n");
    int i = 0;
    int success = 1;

    while (line != NULL && envp[i] != NULL)
    {
        if (strcmp(line, envp[i]) != 0)
        {
            printf("❌ FAIL: Mismatch at index %d\n", i);
            printf("   Expected: \"%s\"\n", envp[i]);
            printf("   Got     : \"%s\"\n", line);
            success = 0;
            break;
        }
        line = strtok(NULL, "\n");
        i++;
    }

    if (success)
    {
        if (envp[i] != NULL)
        {
            printf("❌ FAIL: env() stopped printing early. Missing: \"%s\"\n", envp[i]);
            success = 0;
        }
        else if (line != NULL)
        {
            printf("❌ FAIL: env() printed extra data: \"%s\"\n", line);
            success = 0;
        }
    }

    free(buffer_copy);
    return (success);
}

// --- The Core Test Execution (All logic and edge cases contained inside) ---
int test_env(char **envp)
{
    int pipefd[2];
    int success = 1;
    char *empty_envp[] = { NULL };
    char **current_envp = envp;

    // Loop twice: First pass runs standard envp, second pass runs empty envp
    for (int run = 0; run < 2; run++)
    {
        if (run == 0)
            printf("--- Running Standard OS envp Test ---\n");
        else
        {
            printf("\n--- Running Empty envp (env -i) Test ---\n");
            current_envp = empty_envp;
        }

        if (pipe(pipefd) == -1)
        {
            perror("pipe failed");
            return (0);
        }

        int saved_stdout = dup(STDOUT_FILENO);
        if (saved_stdout == -1)
        {
            perror("dup failed");
            close(pipefd[0]);
            close(pipefd[1]);
            return (0);
        }

        // Redirect stdout to pipe
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        // ==========================================
        // YOUR EXACT CODE STRUCTURE (Uses current_envp iteration target)
        // ==========================================
        t_single_linked_node *lst = env_to_lst(current_envp);
        env(lst);
        ft_single_lstclear(&lst, del_env_node_content);
        // ==========================================

        // Flush and restore stdout immediately
        fflush(stdout);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);

        // Read captured output safely
        char buffer[8192];
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1);
        close(pipefd[0]);

        if (bytes_read < 0)
        {
            perror("read failed");
            return (0);
        }

        // Compare captured string with current target envp array
        if (verify_output(buffer, current_envp))
            printf("✅ PASS: env() output perfectly matches envp!\n");
        else
            success = 0;
    }

    return (success);
}