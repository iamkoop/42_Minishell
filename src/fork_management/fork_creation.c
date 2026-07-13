/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_creation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 14:17:36 by nilsdruon         #+#    #+#             */
/*   Updated: 2026/07/09 19:03:22 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../minishell.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


// number of children -1

static void first_child(int	*fds)
{
	
}
static void middle_children()
{
	
}

static void last_child()
{
		
}

static int parent()
{
	int status = 0;
	int cnt = 10;
	int id[cnt];
	int fds[cnt][2];
	
	for(int i = 0; i < cnt; i++)
	{
		if(pipe(fds[i]) == -1)
			return(1); //need to close all fds on failure
	}
    {
		id[i] = fork();
		if(id[i] == -1)
		for(int i = 0; i <B cnt; i++)
			perror("fork failed");
        if(id[i] == 0)
		{
			if(i == 0)
				first_child(fds[i]);
			else if(i == cnt - 1)
				last_child(fds[i]);
			else
				middle_children(fds[i]);
			exit(i);
		}
 	}
	for(int i = 0; i < cnt; i++)
	{
		for(int j = 0; j < 2; j++)
			close(fds[i][j]);
	}
	for(int i = 0; i < cnt; i++)
		waitpid(id[i], &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
}

int main()
{
	
	return(1);
}
