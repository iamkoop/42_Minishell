/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 15:43:39 by bastalze          #+#    #+#             */
/*   Updated: 2026/07/24 14:42:10 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../minishell.h"

void	ft_get_commandline_input(t_single_linked_node *env);
void	initiate_tokenization(char *input, t_single_linked_node *env);

void	get_commandline_input(t_single_linked_node *env)
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
//			delete_hd_files();
		}
	}
}

void	initiate_tokenization(char *input, t_single_linked_node *env)
{
	t_token_node	token_lst[TOKEN_AMOUNT];
	t_token_iteri	iteri;

	ft_bzero(token_lst, sizeof(t_token_node) * TOKEN_AMOUNT);
	ft_bzero(&iteri, sizeof(t_token_iteri));
	if (tokenization(input, env, token_lst, &iteri))
		delete_hd_files();
}
