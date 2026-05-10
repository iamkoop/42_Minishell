/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 15:43:39 by bastalze          #+#    #+#             */
/*   Updated: 2026/05/10 15:37:14 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>

void	ft_get_commandline_input(char **env)
{
	char	*input;

	while (42)
	{
		input = readline("Minishell> ");
		if (input[0])
		{
			add_history(input);
			ft_parsing(input, env);
			free(input);
		}
	}
}

int	main(char **env)
{
	ft_get_commandline_input(env);
}
