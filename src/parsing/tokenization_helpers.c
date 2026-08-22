/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 10:56:18 by bastalze          #+#    #+#             */
/*   Updated: 2026/08/20 21:33:42 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	add_to_token(char c, t_token_node *token_lst, t_token_iteri *iteri);
int	delimit_token(char *input, t_single_linked_node *env, t_minishell *mini,
		t_token_iteri *iteri);

int	add_to_token(char c, t_token_node *token_lst, t_token_iteri *iteri)
{
	token_lst[iteri->token].token_str[iteri->t] = c;
	if(iteri->t >= TOKEN_STR_SIZE)
	{
		error("exceeding memory limit: Token length \
			   	\nRaise TOKEN_STR_SIZE in minishell.h");
		return (1);
	}
	iteri->t++;
	return (0);
}

int	delimit_token(char *input, t_single_linked_node *env, t_minishell *mini,
				t_token_iteri *iteri)
{
	if(iteri->token >= TOKEN_AMOUNT)
	{
		error("exceeding memory limit: Amount of tokens \
			   	\nRaise TOKEN_AMOUNT in minishell.h");
		return (1);
	}
	iteri->token++;
	iteri->t = 0;
	if (iteri->token >= 2
		&& mini->token_lst[iteri->token - 1].token_type == WORD
		&& mini->token_lst[iteri->token - 2].token_type == HERE_DOC)
	{
		printf("Heredoc entered\n");
		here_doc(input, env, mini, iteri);
	}
	return (0);
}
