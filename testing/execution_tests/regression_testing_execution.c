/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   regression_testing_execution.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 12:38:16 by nildruon          #+#    #+#             */
/*   Updated: 2026/08/12 13:22:58 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "testing.h"

void testing_exec(char **argv, char **envp)
{
	if(argv)
		argv = NULL;
	if(envp)
		envp = NULL;
   	/*env_tests(envp);
	env_manipulation_tests(envp);
	builtins_tests(argv, envp);
	test_exec_command(envp);
	test_redirections_suite();*/
	test_general_cmd_exec();
}