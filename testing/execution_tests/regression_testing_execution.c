/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   regression_testing_execution.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 12:38:16 by nildruon          #+#    #+#             */
/*   Updated: 2026/05/22 17:55:47 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "testing.h"

void testing_exec(char **argv, char **envp)
{
	int i = 0;
    (void)argv;
    i += env_tests(envp);
	
}