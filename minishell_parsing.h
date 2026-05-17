/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parsing.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 15:38:17 by bastalze          #+#    #+#             */
/*   Updated: 2026/05/17 16:31:05 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>

# define TOKEN_STR_SIZE 50
# define TOKENS_AMOUNT 40

//Tokenization:
enum e_token_type
{
	WORD;
	REDIR_IN;
	REDIR_OUT;
	REDIR_OUT_A;
	HERE_DOC;
	PIPE;
} ;

typedef struct s_token_node
{
	enum e_token_type	token_type;
	char				token_str[TOKEN_STR_SIZE];
}		t_token_node;

//Commands for execution:
enum e_redir_type
{
	RE_IN;
	RE_OUT;
	RE_APPEND;
} ;

typedef	struct	s_redir_list
{
	char				*filename;
	int					fd;
	enum e_redir_type	redir_type;
	s_redir_list		*next;
}		t_redir_list;

typedef struct	s_command
{
	char			**argv;
	int				*expand_var;
	t_redir list	*redir;
}		t_command;

typedef struct	s_cmds_list
{
	t_command	*cmd;
	s_cmds_list	*next;
}		t_cmds_list;

void	ft_get_commandline_input(char **env);

#endif
