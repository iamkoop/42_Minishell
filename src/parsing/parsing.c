/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 16:46:40 by bastalze          #+#    #+#             */
/*   Updated: 2026/06/08 10:53:35 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../minishell.h"

int initiate_parsing(char **env, t_token_node *token_lst, t_token_iteri *iteri)
{
	t_cmd_data				cmd_data;
	t_command				*cmd;

	cmd = ft_calloc(1, sizeof(t_command));
	if(!cmd)
		return (1);
	cmd_data.head = ft_single_lstnew(cmd);
	if(!cmd_data-head)
		return (1);
	cmd_data.tail = cmd_data.head;
	assert(token_lst != NULL);
	assert(cmd_data != NULL);
	if (parsing(env, token_lst, iteri, &cmd_data))
		return (1);
	return(0);
}

int	parsing(char **env, t_token_node *token_lst, t_token_iteri *iteri,
		t_cmd_data *cmd_data)
{
	int	is_redir;

	bzero(&iteri, sizeof(t_token_iteri));
	while (token_lst[iteri->token].token_type)
	{
		is_redir = 0;
		if (is_redirection(token_lst, iteri))
		{
			if (redirect(env, token_lst, iteri, cmd_data))
				return (1);
			is_redir = 1;
		}
		if (is_syntax_error(token_lst, iteri))
		{
			error("syntax error unexpected token");
			return (1);
		} 
		if (!is_redir)
		{
			if (word_or_pipe(env, token_lst, iteri, cmd_data))
				return (1);
		}
		iteri->token++;
	}
	return (0);
}

int	is_redirection(t_token_node *token_lst, t_token_iteri *iteri)
{
	if (iteri->token != 0 &&
			(token_lst[iteri->token - 1].token_type == REDIR_IN 
			|| token_lst[iteri->token - 1].token_type == REDIR_OUT
			|| token_lst[iteri->token - 1].token_type == REDIR_OUT_A
			|| token_lst[iteri->token - 1].token_type == HERE_DOC)
			&& token_lst[iteri->token].token_type == WORD)
		return (1);
	return (0);
}

int	redirect(t_single_linked_node *env, t_token_node *token_lst, t_token_iteri *iteri,
		t_cmd_data *cmd_data)
{
	t_redir_list		*curr_redir;
	t_command		*tmp_cmd;
	t_single_linked_node	*tmp_redir;
	char			word[WORD_AMOUNT][WORD_STR_SIZE]

	tmp_cmd = (t_command *)cmd_data->tail->content;
	curr_redir = calloc(1, sizeof(t_redir_list));
	if (!curr_redir)
		return (1);
	redir_type_assignment(curr_redir, token_lst);
	if(quote_rm_var_expan(token_lst[iteri->token]->token_str, word, env, false))
		return 1;
	if(word[1][0] != 0)
	{
		error("ambiguous redirect");
		return (1);
	}
	curr_redir->filename = calloc(1, ft_strlen(word[0]) + 1);
	if (!curr_redir->filename)
		return (1);
	ft_strlcpy(curr_redir->filename, word[0], ft_strlen(word[0]) + 1);
	tmp_redir = ft_single_lstnew(curr_redir);
	if (!tmp_redir)
		return (1);
	if (!tmp_cmd->redir)
		tmp_cmd->redir = tmp_redir;
	else
		ft_lstadd_back_single_linked(&tmp_cmd->redir, tmp_redir);
	return (0);
}

void	redir_type_assignment(t_redir_list *curr_redir, t_token_node *token_lst)
{
	if (token_lsti[iteri->token]->token_type == REDIR_IN)
		curr_redir->redir_type == IN;
	else if (token_lst[iteri->token]->token_type == REDIR_OUT)
		curr_redir->redir_type == OUT;
	else if (token_lst[iteri->token]->token_type == REDIR_OUT_A)
		curr_redir->redir_type == APPEND;
	else if (token_lst[iteri->token]->token_type == HERE_DOC)
		curr_redir->redir_type == HERE;
}

int	syntax_error(t_token_node *token_lst, t_token_iteri *iteri)
{
	if (token_lst[0].token_type == PIPE)
		return (1);
	else if (iteri->token != 0 &&
				(token_lst[iteri->token - 1].token_type == REDIR_IN
				|| token_lst[iteri->token - 1].token_type == REDIR_OUT
				|| token_lst[iteri->token - 1].token_type == REDIR_OUT_A
				|| token_lst[iteri->token - 1].token_type == HERE_DOC
				|| token_lst[iteri->token - 1].token_type == PIPE)
				&& (token_lst[iteri->token].token_type == REDIR_IN
				|| token_lst[iteri->token].token_type == REDIR_OUT
				|| token_lst[iteri->token].token_type == REDIR_OUT_A
				|| token_lst[iteri->token].token_type == HERE_DOC
				|| token_lst[iteri->token].token_type == PIPE))
		return (1);
	else if ((token_lst[iteri->token].token_type == REDIR_IN
				|| token_lst[iteri->token].token_type == REDIR_OUT
				|| token_lst[iteri->token].token_type == REDIR_OUT_A
				|| token_lst[iteri->token].token_type == HERE_DOC
				|| token_lst[iteri->token].token_type == PIPE)
				&& token_lst[iteri->token + 1].token_type == 0)
		return (1);
	return (0);
}

int	word_or_pipe(char **env, t_token_node *token_lst, t_token_iteri *iteri,
		t_cmd_data *cmd_data)
{
	char	word[WORD_AMOUNT][WORD_STR_SIZE];
	
	ft_bzero(word, (WORD_AMOUNT * WORD_STR_SIZE));
	if (token_lst[iteri->token].token_type == WORD)
	{
		if(quote_rm_var_expan(token_lst[iteri->token].token_str, word, env, false))
			return (1);
		if(transfer_word(cmd_data, word))
		{
			perror();
			return (1);
		}
	}
	else if (token_lst[iteri->token].token_type == PIPE)
	{
		if(delimit_command(cmd_data))
			return (1);
	}
}

int	transfer_word(t_cmd_data *cmd_data, word[WORD_AMOUNT][WORD_STR_SIZE])
{
	int		i;
	int		j;
	size_t		argv_i;
	size_t		argv_j;
	char		**tmp_argv;
	t_command	*tmp_cmd;
	
	tmp_cmd = (t_command *)cmd_data->tail->content;
	tmp_argv = ft_calloc(ft_2darraylen(word) + ft_strarraylen(tmp_cmd->argv) + 1, sizeof(char *));
	if(!tmp_argv)
		return (1);
	argv_i = 0;
	while(tmp_cmd->argv[argv_i])
	{
		tmp_argv[argv_i] = calloc(ft_strlen(tmp_cmd->argv[argv_i]), 1);
		if(!tmp_argv[argv_i])
			return (1);
		argv_j = 0;
		while(tmp_cmd->arg[argv_i][argv_j])
		{
			tmp_arv[argv_i][argv_j] = tmp_cmd->arg[argv_i][argv_j];
			argv_j++;
		}
		tmp_arv[argv_i][argv_j] = 0;
		argv_i++;
	}
	i = 0;
	while(i < WORD_AMOUNT && word[i][0] != 0)
	{
		tmp_argv[argv_i + i] = calloc(ft_strlen(word[i], 1));
		if(!tmp_argv[argv_i + i])
			return (1);
		j = 0;
		while(word[i][j])
		{
			tmp_arv[argv_i + i][j] = word[i][j];
			j++;
		}
		tmp_arv[argv_i + i][j] = 0;
		i++;
	}
	tmp_arv[argv_i + i] = NULL;
	free_strarray(tmp_cmd->argv);
	tmp_cmd->argv = tmp_argv;
	return (0);
}

void	free_strarray(char **array)
{
	int i;

	i = 0;
	while(array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

size_t	ft_2darraylen(char word[WORD_AMOUNT][WORD_STR_SIZE])
{
	size_t i;

	i = 0;
	while (i < WORD_AMOUNT && word[i][0] != 0)
        	i++;
	return (i);
}

size_t	ft_strarraylen(char **argv)
{
	size_t	i;

	i = 0;
	while(argv[i])
		i++;
	return (i);
}

int	delimit_command(t_cmd_data *cmd_data)
{
	t_command		*new_cmd;
	t_single_linked_node	new_node_ptr;

	new_cmd = calloc(1, sizeof(t_command));
	if(!new_cmd)
		return (1);
	new_node_ptr = ft_single_lstnew(new_cmd);
	if(!tmp_node_ptr)
		return (1);
	ft_lstadd_back_single_linked(&cmd_data->head, new_node_ptr);
	cmd_data->tail = new_node_ptr;
	return (0);
}
