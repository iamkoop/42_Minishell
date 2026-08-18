/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_testing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 14:51:07 by bastalze          #+#    #+#             */
/*   Updated: 2026/08/18 14:52:13 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "testing.h"

void	main_testing(char **argv, char **envp)
{
	/*
	if (!*envp)
	{
		if (default_environment())
			return (1);
	}
	*/
	printf("Testing Minishell\n");
	printf("---------------------------\n");
	printf("========================================\n");
	printf("|          ALL EXECUTION TEST          |\n");
	printf("========================================\n");
	testing_exec(argv, envp);
	printf("\n");
	printf("========================================\n");
	printf("|          ALL PARSING TESTS           |\n");
	printf("========================================\n");
//	t_single_linked_node *env = creating_fake_env();
//	testing_parsing(env);
}