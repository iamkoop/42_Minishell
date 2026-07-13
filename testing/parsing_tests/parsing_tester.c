/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_tester.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 16:45:00 by bastalze          #+#    #+#             */
/*   Updated: 2026/06/04 16:47:38 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../minishell.h"

void	tokenization_testing(t_token_node *token_lst, char **env)
{
	int i;

	i = 0;
	env = NULL;
	printf("TOKEN TESTING\n");
	printf("--------------------------\n");
	while(token_lst[i].token_str[0])
	{
		printf("%u\n%s$\n\n", token_lst[i].token_type, token_lst[i].token_str);
		i++;
	}
	printf("End of TOKEN TESTING\n");
}

void	test_quote_removal()
{
	char	word1[26] = "\"Test 1 \'quote\' re\"moval";
	char	word2[26] = "\'Test 2 \"quote\" re\'moval";

	printf("QUOTE REMOVAL TESTING\n");
	printf("--------------------------\n");
	printf("Test double quotes\nOriginal:\n%s\nAfter removal:\n%s\n", 
			word1, quote_removal(word1));
	printf("Test single quotes\nOriginal:\n%s\nAfter removal:\n%s\n", 
			word2, quote_removal(word2));
	printf("--------------------------\n");
}

/*
void	test_heredoc_creation()
{
	int 	i;
	char	filename[16];

	i = 0;
	while (i < 101)
	{
		if (heredoc_filename_creation(filename))
			break ;
		printf("Heredoc filename%d: %s\n", i, filename);
		i++;
	}
}
*/
void	testing_parsing(char **env)
{
//	char *input;

//	input = "why<|     >> 'dont we'> <<|helllo        sdd";
	test_quote_removal();
//	test_heredoc_creation();
	get_commandline_input(env);
//	initiate_tokenization(input}
}
