/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:50:14 by nildruon          #+#    #+#             */
/*   Updated: 2026/06/09 22:48:44 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "testing.h"

int builtins_tests(char **argv, char **envp)
{
	(void)argv;
	/*envp = NULL;
	if(envp != NULL)
		exit(1); */
	/* test_env(envp);
	test_echo();
	test_builtin_exit(envp);
	test_pwd(); */
	test_cd(envp);
	return(1);
}