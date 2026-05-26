/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 15:43:39 by bastalze          #+#    #+#             */
/*   Updated: 2026/05/26 10:20:13 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../minishell.h"

void	ft_get_commandline_input(char **env);
void	initiate_tokenization(char *input, char **env);

void	get_commandline_input(char **env)
{
	char	*input;

	while (42)
	{
		input = readline("Minishell> ");
		if (input[0])
		{
			add_history(input);
			initiate_tokenization(input, env);
			free(input);
		}
	}
}

void	initiate_tokenization(char *input, char **env)
{
	t_token_node	token_lst[TOKEN_AMOUNT];
	t_token_iteri	iteri;

	bzero(token_lst, sizeof(t_token_node) * TOKEN_AMOUNT);
	bzero(&iteri, sizeof(t_token_iteri));
	tokenization(input, env, token_lst, &iteri);
}
