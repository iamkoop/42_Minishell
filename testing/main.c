/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 11:51:03 by nildruon          #+#    #+#             */
/*   Updated: 2026/05/26 13:23:12 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "testing.h"

int main(int argc, char	**argv, char	**envp)
{
	if(argc == -1)
		return(-1);
	if(!argv || !*argv)
		return(1);
	if(!envp || !*envp)
		return(1);	
	printf("Testing Minishell\n");
	printf("---------------------------\n");
	printf("All Exectution Tests\n");
	testing_exec(argv, envp);
}