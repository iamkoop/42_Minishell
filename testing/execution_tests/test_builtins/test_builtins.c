/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:50:14 by nildruon          #+#    #+#             */
/*   Updated: 2026/08/17 18:46:20 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "testing.h"

int builtins_tests(char **argv, char **envp)
{
	(void)argv;
	//(void)envp;
	//env_tests(envp);
	//test_echo();
	//test_builtin_exit(envp);
	//test_pwd();
	//test_cd(envp);
	//test_unset(envp);
	test_export(envp);
	return(1);
}