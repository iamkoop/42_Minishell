/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 15:38:17 by bastalze          #+#    #+#             */
/*   Updated: 2026/05/26 11:10:11 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef MINISHELL_H
# define MINISHELL_H
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

//Delete after integrating libft!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# include <strings.h>

# define TOKEN_STR_SIZE 50
# define TOKEN_AMOUNT 50
# define HD_DELIMITER_LEN 50 

//Tokenization:
enum e_token_type
{
	DEFAULT,
	WORD,
	REDIR_IN,
	REDIR_OUT,
	REDIR_OUT_A,
	HERE_DOC,
	PIPE,
} ;

typedef struct s_token_node
{
	enum e_token_type	token_type;
	char				token_str[TOKEN_STR_SIZE];
}		t_token_node;

typedef struct s_token_iteri
{
	int	token;
	int	t;
	int i;
}		t_token_iteri;

//Commands for execution:
enum e_redir_type
{
	RE_IN,
	RE_OUT,
	RE_APPEND,
} ;

typedef struct s_redir_list
{
	char				*filename;
	int					fd;
	enum e_redir_type	redir_type;

}		t_redir_list;

typedef struct s_command
{
	char			**argv;
	int				*expand_var;
	t_redir_list	*redir;
}		t_command;

typedef struct s_cmds_list
{
	t_command			*cmd;
	struct s_cmds_list	*next;
}		t_cmds_list;

//Functions of minishell:
void	get_commandline_input(char **env);
int		tokenization(char *input, char **env, t_token_node *token_lst,
			t_token_iteri *iteri);
void	add_to_token(char c, t_token_node *token_lst, t_token_iteri *iteri);
void	delimit_token(char *input, char **env, t_token_node *token_lst,
				t_token_iteri *iteri);

//Testers:
void	tokenization_testing(t_token_node *token_lst, char **env);
void	initiate_tokenization(char *input, char **env);

#endif
