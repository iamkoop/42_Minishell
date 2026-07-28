/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_var_and_expand.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 12:45:00 by bastalze          #+#    #+#             */
/*   Updated: 2026/07/28 12:45:04 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../minishell.h"

int			find_var(char *var, char word[WORD_AMOUNT][WORD_STR_SIZE],
				t_quote_iteri *iteri, t_single_linked_node *env);
static int	variable_expansion(char word[WORD_AMOUNT][WORD_STR_SIZE],
				t_quote_iteri *iteri, t_env_var *tmp_content);
static int	field_splitting(char word[WORD_AMOUNT][WORD_STR_SIZE],
				t_quote_iteri *iteri);
static int	add_variable_char(char word[WORD_AMOUNT][WORD_STR_SIZE],
				t_quote_iteri *iteri, int i, t_env_var *tmp_content);

int	find_var(char *var, char word[WORD_AMOUNT][WORD_STR_SIZE],
		t_quote_iteri *iteri, t_single_linked_node *env)
{
	t_single_linked_node	*tmp_node;
	t_env_var				*tmp_content;

	tmp_node = env;
	while (tmp_node)
	{
		tmp_content = (t_env_var *)tmp_node->content;
		if (!ft_strncmp(tmp_content->key, var, ft_strlen(var)))
		{
			if (variable_expansion(word, iteri, tmp_content))
				return (1);
		}
		tmp_node = tmp_node->next;
	}
	return (0);
}

static int	variable_expansion(char word[WORD_AMOUNT][WORD_STR_SIZE],
		t_quote_iteri *iteri, t_env_var *tmp_content)
{
	int	i;

	i = 0;
	while (tmp_content->value[i])
	{
		if ((tmp_content->value[i] == ' ' || tmp_content->value[i] == '\t'
				|| tmp_content->value[i] == '\n') && iteri->quoted == false
			&& iteri->heredoc == false)
		{
			if (field_splitting(word, iteri))
				return (1);
		}
		else
		{
			if (add_variable_char(word, iteri, i, tmp_content))
				return (1);
		}
		i++;
	}
	return (0);
}

static int	field_splitting(char word[WORD_AMOUNT][WORD_STR_SIZE],
		t_quote_iteri *iteri)
{
	if (iteri->wj != 0)
	{
		word[iteri->wi][iteri->wj] = 0;
		if (iteri->wi + 1 >= WORD_AMOUNT)
		{
			error("exceeding memory limit: Amount of words \
				\nRaise WORD_AMOUNT in minishell.h");
			return (1);
		}
		iteri->wi++;
		iteri->wj = 0;
	}
	return (0);
}

static int	add_variable_char(char word[WORD_AMOUNT][WORD_STR_SIZE],
		t_quote_iteri *iteri, int i, t_env_var *tmp_content)
{
	word[iteri->wi][iteri->wj] = tmp_content->value[i];
	if (iteri->wj + 1 >= WORD_STR_SIZE)
	{
		error("exceeding memory limit: Word length \
			\nRaise WORD_STR_SIZE in minishell.h");
		return (1);
	}
	iteri->wj++;
	return (0);
}