/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   regression_testing_execution.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 12:38:16 by nildruon          #+#    #+#             */
/*   Updated: 2026/07/16 17:08:44 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "testing.h"

void testing_exec(char **argv, char **envp)
{
	argv = NULL;
	envp = NULL;
   	/*env_tests(envp);
	env_manipulation_tests(envp);
	builtins_tests(argv, envp);
	test_exec_command(envp);*/
	test_redirections_suite();
}