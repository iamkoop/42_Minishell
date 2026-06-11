/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:50:10 by nildruon          #+#    #+#             */
/*   Updated: 2026/06/11 19:19:13 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "testing.h"

static void free_local_matrix(char **matrix)
{
    int i = 0;
    if (!matrix)
        return;
    while (matrix[i])
    {
        free(matrix[i]);
        i++;
    }
    free(matrix);
}

static char **duplicate_envp(char **envp)
{
    int count = 0;
    while (envp && envp[count])
        count++;
    
    char **copy = malloc(sizeof(char *) * (count + 1));
    if (!copy)
        return (NULL);
        
    for (int i = 0; i < count; i++)
    {
        copy[i] = strdup(envp[i]);
        if (!copy[i])
        {
            free_local_matrix(copy);
            return (NULL);
        }
    }
    copy[count] = NULL;
    return (copy);
}

static int verify_output(const char *buffer, char **envp)
{
    char *buffer_copy = strdup(buffer);
    if (!buffer_copy)
        return (0);

    char *line = strtok(buffer_copy, "\n");
    int i = 0;
    int success = 1;

    while (line != NULL && envp[i] != NULL)
    {
        if (strcmp(line, envp[i]) != 0)
        {
            success = 0;
            break;
        }
        line = strtok(NULL, "\n");
        i++;
    }
    if (success && (envp[i] != NULL || line != NULL))
        success = 0;

    free(buffer_copy);
    return (success);
}

int env_tests(char **envp)
{
    int pipefd[2];
    int success = 1;
    char *empty_envp[] = { NULL };
    char **current_envp;

    // Isolate host envp into clean heap memory to protect against mutation/segfaults
    char **safe_envp = duplicate_envp(envp);
    if (!safe_envp)
        return (0);

    for (int run = 0; run < 2; run++)
    {
        if (run == 0)
        {
            printf("--- Running Standard OS envp Test via env() ---\n");
            current_envp = safe_envp;
        }
        else
        {
            printf("\n--- Running Empty envp (env -i) Test via env() ---\n");
            current_envp = empty_envp;
        }

        if (pipe(pipefd) == -1)
        {
            free_local_matrix(safe_envp);
            return (0);
        }

        int saved_stdout = dup(STDOUT_FILENO);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        t_single_linked_node *lst = env_to_lst(current_envp);
        env(lst);
        ft_single_lstclear(&lst, del_env_node_content);

        fflush(stdout);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);

        char *buffer = calloc(65536, 1);
        if (buffer)
        {
            ssize_t bytes = read(pipefd[0], buffer, 65535);
            (void)bytes;
            if (verify_output(buffer, current_envp))
                printf("✅ PASS: env() output perfectly matches input matrix layout!\n");
            else
            {
                printf("❌ FAIL: env() output mismatch!\n");
                success = 0;
            }
            free(buffer);
        }
        close(pipefd[0]);
    }

    free_local_matrix(safe_envp);
    return (success);
}