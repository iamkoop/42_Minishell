/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 15:50:14 by username          #+#    #+#             */
/*   Updated: 2026/08/30 17:21:58 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "testing.h"

int	builtins_tests(char **argv, char **envp)
{
	(void) argv;
	//(void) envp;
	//env_tests(envp); done and dusted
	//test_echo(); done
	//test_builtin_exit(envp); done
	//test_pwd(); done
	//test_cd(envp); done Target path lacks execute permissions [Exit Status Mismatch] to test funcheck passes but not---
	//test_unset(envp); funcheck and valgrind
	test_export(envp);
	return (1);
}
