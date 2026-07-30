/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 11:51:03 by nildruon          #+#    #+#             */
/*   Updated: 2026/07/30 14:33:45 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "testing.h"
t_single_linked_node *creating_fake_env();


int main(int argc, char	**argv, char **envp)
{
	if(argc == -1)
		return(-1);
	if(!argv || !*argv)
		return(1);
	if(!envp || !*envp)
		return(1);	
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
	t_single_linked_node *env = creating_fake_env();
//	testing_parsing(env);
	get_commandline_input(env);
}

t_single_linked_node *creating_fake_env()
{
	t_env_var				*env_struct;

	env_struct = malloc(sizeof(t_env_var));
	env_struct->key = strdup("VAR");
    env_struct->value = strdup("Hamster cage");
	return (ft_single_lstnew(env_struct));
}
