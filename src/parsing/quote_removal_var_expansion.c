/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal_var_expansion.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 16:27:55 by bastalze          #+#    #+#             */
/*   Updated: 2026/07/21 15:02:28 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include  "../../minishell.h"

int	quote_rm_var_expan(char *s, char word[WORD_AMOUNT][WORD_STR_SIZE],
		t_single_linked_node *env, bool heredoc);
int	quote_mode(char *s, char word[WORD_AMOUNT][WORD_STR_SIZE],
		t_quote_iteri *iteri, t_single_linked_node *env);
int	variable_expansion(char *s, char word[WORD_AMOUNT][WORD_STR_SIZE],
		t_quote_iteri *iteri, t_single_linked_node *env);
int	is_name(int i, char c);
int	find_var(char *var, char word[WORD_AMOUNT][WORD_STR_SIZE],
		t_quote_iteri *iteri, t_single_linked_node *env);


int	quote_rm_var_expan(char *s, char word[WORD_AMOUNT][WORD_STR_SIZE],
		t_single_linked_node *env, bool heredoc)
{
	t_quote_iteri	iteri;

	ft_bzero(&iteri, sizeof(t_quote_iteri));
	if(heredoc == true)
		iteri.heredoc = true;
	while(s[iteri.i])
	{
		if((s[iteri.i] == '\'' || s[iteri.i] == '\"') && heredoc == false)
		{
			if(quote_mode(s, word, &iteri, env))
				return (1);
		}
		else if(s[iteri.i] == '$')
		{
			if(variable_expansion(s, word, &iteri, env))
				return (1);
		}
		else
		{
			word[iteri.wi][iteri.wj] = s[iteri.i];
			if(iteri.wj + 1 >= WORD_STR_SIZE)
			{
				error("exceeding memory limit: Word length \
					\nRaise WORD_STR_SIZE in minishell.h");
				return (1);
			}
			iteri.wj++;
			iteri.i++;
		}
	}
	word[iteri.wi][iteri.wj] = 0;
	return (0);
}

int	quote_mode(char *s, char word[WORD_AMOUNT][WORD_STR_SIZE], t_quote_iteri *iteri, t_single_linked_node *env)
{
	char	c;

	c = s[iteri->i];
	iteri->quoted = true;
	assert(s[iteri->i] == '\'' || s[iteri->i] == '\"');
	iteri->i++;
	while(s[iteri->i] && s[iteri->i] != c)
	{
		if(s[iteri->i] == '$' && c == '\"')
		{
			if(variable_expansion(s, word, iteri, env))
				return (1);
		}
		else
		{
			word[iteri->wi][iteri->wj] = s[iteri->i];
			if(iteri->wj + 1 >= WORD_STR_SIZE)
			{
				error("exceeding memory limit: Word length \
									\nRaise WORD_STR_SIZE in minishell.h");
				return (1);
			}
			iteri->i++;
			iteri->wj++;
		}
	}
	assert(s[iteri->i] == '\'' || s[iteri->i] == '\"');
	iteri->i++;
	iteri->quoted = false;
	return (0);
}

int	variable_expansion(char *s, char word[WORD_AMOUNT][WORD_STR_SIZE], t_quote_iteri *iteri, t_single_linked_node *env)
{
	char	var[WORD_STR_SIZE];
	int	v;

	bzero(var, WORD_STR_SIZE - 1);
	assert(s[iteri->i] == '$');
	iteri->i++;
	v = 0;
	while (s[iteri->i] && is_name(v, s[iteri->i]))
	{
		var[v] = s[iteri->i];
		iteri->i++;
		v++;
	}
	if(v != 0)
	{
		var[v] = 0;
		if(find_var(var, word, iteri, env))
			return (1);
	}
	else if(s[iteri->i] == '?')
	{
		word[iteri->wi][iteri->wj] = '$';
		word[iteri->wi][++iteri->wj] = '?';
		if(iteri->wj + 1 >= WORD_STR_SIZE)
		{
			error("exceeding memory limit: Word length \
				\nRaise WORD_STR_SIZE in minishell.h");
			return (1);
		}
		iteri->wj++;
		iteri->i++;
//		better to directly get the exit status and put it in!
	}
	else
	{
		assert(v == 0);
		word[iteri->wi][iteri->wj] = '$';
		if(iteri->wj + 1 >= WORD_STR_SIZE)
		{
			error("exceeding memory limit: Word length \
				\nRaise WORD_STR_SIZE in minishell.h");
			return (1);
		}
		iteri->wj++;
	}
	return (0);
}
// no safety net for max v needed cause s is maximum WORD_STR_SIZE so var can't be bigger

int	is_name(int i, char c)
{
	if(i == 0 && c >= '0' && c <= '9')
		return (1);
	if(c == '_' ||
		(c >= '0' && c <= '9') ||
		(c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

int	find_var(char *var, char word[WORD_AMOUNT][WORD_STR_SIZE],
		t_quote_iteri *iteri, t_single_linked_node *env)
{
	t_single_linked_node	*tmp_node;
	t_env_var				*tmp_content;
	int						i;

	tmp_node = env;
	while(tmp_node)
	{
		tmp_content = (t_env_var *)tmp_node->content;
		if(!ft_strncmp(tmp_content->key, var, ft_strlen(var)))
		{
			i = 0;
			while(tmp_content->value[i])
			{
				if((tmp_content->value[i] == ' ' || tmp_content->value[i] == '\t'
					|| tmp_content->value[i] == '\n') && iteri->quoted == false
					&& iteri->heredoc == false)
				{
					if(iteri->wj != 0)
					{
						word[iteri->wi][iteri->wj] = 0;
						if(iteri->wi + 1 >= WORD_AMOUNT)
						{
							error("exceeding memory limit: Amount of words \
								\nRaise WORD_AMOUNT in minishell.h");
							return (1);
						}
						iteri->wi++;
						iteri->wj = 0;
					}
				}
				else
				{
					word[iteri->wi][iteri->wj] = tmp_content->value[i];
					if(iteri->wj + 1 >= WORD_STR_SIZE)
					{
						error("exceeding memory limit: Word length \
							\nRaise WORD_STR_SIZE in minishell.h");
						return (1);
					}
					iteri->wj++;
				}
				i++;
			}
			return (0);
		}
		tmp_node = tmp_node->next;
	}
	return (0);
}
