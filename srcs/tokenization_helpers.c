/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 10:56:18 by bastalze          #+#    #+#             */
/*   Updated: 2026/05/26 11:26:35 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../minishell.h"

void	add_to_token(char c, t_token_node *token_lst, t_token_iteri *iteri);
void	delimit_token(char *input, char **env, t_token_node *token_lst,
				t_token_iteri *iteri);

void	add_to_token(char c, t_token_node *token_lst, t_token_iteri *iteri)
{
	token_lst[iteri->token].token_str[iteri->t] = c;
	iteri->t++;
}

void	delimit_token(char *input, char **env, t_token_node *token_lst,
				t_token_iteri *iteri)
{
	env = NULL;
	input = NULL;
	iteri->token++;
	iteri->t = 0;
	if (iteri->token >= 2
		&& token_lst[iteri->token - 1].token_type == WORD
		&& token_lst[iteri->token - 2].token_type == HERE_DOC)
		printf("Heredoc entered\n");
}
