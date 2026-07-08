/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal_var_expansion.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 16:27:55 by bastalze          #+#    #+#             */
/*   Updated: 2026/07/07 17:21:49 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include  "../../minishell.h"

char	**quote_rm_var_expan(char *s, t_single_linked_node *env)
{
	t_quote_iteri	iteri;
	static char	word[WORD_AMOUNT][WORD_STR_SIZE];

	bzero(&iteri, sizeof(t_quote_iteri));
	while(s[iteri.i])
	{
		if(s[iteri.i] == '\'' || s[iteri.i] == '\"')
			if(quote_mode(s, words, &iteri, env))
				return (NULL);
		else if(s[iteri.i] == '$')
			variable_expansion(s, words, &iteri, env);
		else
		{
			words[iteri->wi][iteri->wj] = s[iteri->i];
			iteri->wj++;
			iteri->i++;
		}
	}
	word[iteri.wi][iteri.wj] = 0;
	return (word);
}

int	quote_mode(char *s, char **word, t_quote_iteri *iteri, t_single_linked_node *env)
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
			if(variable_expansion(s, words, iteri, env))
				return (1);
		}
		else
			word[iteri->wi][iteri->wj] = s[iteri->i];
		if(iteri->wj + 1 >= WORD_STR_SIZE)
		{
			error("exceeding memory limit: Word token length \
                                \nTo use a longer delimiter raise WORD_STR_SIZE \
                                in minishell.h");
			return (1);
		}
		iteri->wj++;
		iteri->i++;
	}
	assert(s[iteri->i] == '\'' || s[iteri->i] == '\"');
	iteri->i++;
	iteri->quoted = false;
	return (0);
}

void	variable_expansion(char *s, char **word, t_quote_iteri *iteri, t_single_linked_node *env)
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
		find_var(var, word, iteri, env);
	}
	else if(s[iteri->i] == '?')
	{
		word[iteri->wi][iteri->wj] = '$';
		word[iteri->wi][++iteri->wj] = '?';
		iteri->wj++;
		iteri->i++;
//		better to directly get the exit status and put it in!
	}
	else
	{
		assert(v == 0);
		word[iteri->wi][iteri->wj] = '$';
		iteri->wj++;
	}
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
}

void find_var(char *var, char **word, t_quote_iteri *iteri, t_single_linked_node *env)
{
	t_single_linked_node	*tmp_node;
	t_env_var		*tmp_content;
	int			i;

	tmp_node = env;
	while(tmp_node)
	{
		tmp_content = (t_env_var *)tmp_node->content;
		if(!ft_strncmp(tmp_content->key, var, ft_strlen(var)))
		{
			i = 0;
			while(tmp_content->value[i])
			{
				if(tmp_content->value[i] == ' ' || tmp_content->value[i] == '\t' || tmp_content->value[i] == '\n'
					&& iteri->quotes == false)
				{
					if(iteri->wj != 0)
					{
						word[iteri->wi][iteri->wj] = 0;
						iteri->wi++;
						iteri->wj = 0;
					}
				}
				word[iteri->wi][iteri->wj] = tmp_content->value[i];
				iteri->wj++;
				i++;
			}
			return ;
		}
		tmp_node = tmp_node->next;
	}
}
