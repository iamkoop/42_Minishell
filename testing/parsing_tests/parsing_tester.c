/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_tester.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 16:45:00 by bastalze          #+#    #+#             */
/*   Updated: 2026/07/30 14:21:23 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../minishell.h"
#include "../testing.h"

/*
void	tokenization_ut_compare(char *test_input, t_token_node *token_lst,
			t_token_node *expected_token_lst, int result)
{
	int i = 0;

	if(result == 1)
	{
		if(expected_token_lst[i].token_type != 0)
			printf("❌FAIL: %s, unexpected error occured\n");
		else
			printf("✅SUCCESS: %s, expected error occured\n");
	}
	else
	{
		while(token_lst[i].token_type)
		{
			if(token_lst[i].token_type != expected_token_lst[i].token_type
				|| strcmp(token_lst[i].token_str, expected_token_lst[i].token_str))
			{
				printf("❌FAIL: %s, Token NBR %d\nExpected: type: %d, string: %s\n
					Got: type: %d, string: %s\n", test_input, i + 1, 
					expected_token_lst[i].token_type, expected_token_lst[i].token_str,
					token_lst[i].token_type, token_lst[i].token_str);
				return ;
			}
			i++;
		}
		printf("✅SUCCESS: %s\n", test_input);
	}
}

void	tokenization_unit_tetsing(t_single_linked_node *env)
{
	char			*test_input;
	t_token_node    expected_token_lst[TOKEN_AMOUNT];
	t_token_node    token_lst[TOKEN_AMOUNT];
	t_token_iteri   iteri;

	ft_bzero(token_lst, sizeof(t_token_node) * TOKEN_AMOUNT);
	ft_bzero(test_token_lst, sizeof(t_token_node) * TOKEN_AMOUNT);
	ft_bzero(&iteri, sizeof(t_token_iteri));
	int result = tokenization(test_input, env, token_lst, &iteri);
}
*/
/*
void	tokenization_testing(t_token_node *token_lst, t_single_linked_node *env)
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
*/
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
void	testing_parsing(t_single_linked_node *env)
{
//	char *input;

//	input = "why<|     >> 'dont we'> <<|helllo        sdd";
//	test_quote_removal();
//	test_heredoc_creation();
//	get_commandline_input(env);
//	initiate_tokenization(input}
	tokenization_unit_testing(env);
	qrve_unit_testing(env);
	parsing_unit_testing();
}
