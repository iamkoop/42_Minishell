/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 15:38:17 by bastalze          #+#    #+#             */
/*   Updated: 2026/07/28 12:48:11 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef MINISHELL_H
# define MINISHELL_H
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "42_Libft/libft.h"
# include <errno.h>
# include <stdbool.h>
# include <dirent.h>

//Delete after testing!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <assert.h>

# define TOKEN_AMOUNT 10
# define TOKEN_STR_SIZE 20
# define HD_DELIMITER_LEN 50 
# define WORD_AMOUNT 40
# define WORD_STR_SIZE 50

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
	int		token;
	int		t;
	int		i;
}		t_token_iteri;

//Parsing:
enum e_redir_type
{
	IN,
	OUT,
	APPEND,
	HERE,
} ;

typedef struct s_redir_list
{
	char				*filename;
	int					fd;
	enum e_redir_type	redir_type;
}		t_redir_list;

typedef struct s_command
{
	char					**argv;
	t_single_linked_node	*redir;
}		t_command;

typedef struct	s_cmd_data
{
	t_single_linked_node	*head;
	t_single_linked_node	*tail;
}		t_cmd_data;

//Quote_removal_variable_expansion
typedef struct s_quote_iteri
{
	int		i;
	int		wi;
	int		wj;
	bool	quoted;
	bool	heredoc;
}		t_quote_iteri;

//env_var_struct
typedef struct s_env_var
{
	char				*key;
	char				*value;
}					t_env_var;

//FUNCTIONS
//environment conversion
t_single_linked_node	*env_to_lst(char	**envp);
void					del_env_node_content(void	*content);
char					**env_to_char_arr(t_single_linked_node	*lst);
//builtins
void					env(t_single_linked_node	*envp);
void					echo(char	**input);
//PARSING PART
void	get_commandline_input(t_single_linked_node *env);
//tokenization
int		tokenization(char *input, t_single_linked_node *env, t_token_node *token_lst,
			t_token_iteri *iteri);
int		here_or_append(char *input, t_single_linked_node *env,
			t_token_node *token_lst, t_token_iteri *iteri);
int		operators1(char *input, t_single_linked_node *env,
			t_token_node *token_lst, t_token_iteri *iteri);
int		operators2(char *input, t_single_linked_node *env,
			t_token_node *token_lst, t_token_iteri *iteri);
int		redirections(char *input, t_single_linked_node *env,
			t_token_node *token_lst, t_token_iteri *iteri);
int		add_to_token(char c, t_token_node *token_lst, t_token_iteri *iteri);
int		delimit_token(char *input, t_single_linked_node *env,
			t_token_node *token_lst, t_token_iteri *iteri);
//heredoc
char	*quote_removal(char *delimiter);
int		here_doc(char *input, t_single_linked_node *env, t_token_node *token_lst,
			t_token_iteri *iteri);
int		adding_heredoc_into_file(int fd, bool expansion, char *delimiter,
				t_single_linked_node *env);
//error and exit functions
void	error(char *message);
void	delete_hd_files();
// parsing
int		initiate_parsing(t_single_linked_node *env, t_token_node *token_lst,
			t_token_iteri *iteri);
//quote removal and variable expansion
int		quote_rm_var_expan(char *s, char word[WORD_AMOUNT][WORD_STR_SIZE],
			t_single_linked_node *env, bool heredoc);
int		dollar_found(char *s, char word[WORD_AMOUNT][WORD_STR_SIZE],
			t_quote_iteri *iteri, t_single_linked_node *env);
int		find_var(char *var, char word[WORD_AMOUNT][WORD_STR_SIZE],
			t_quote_iteri *iteri, t_single_linked_node *env);

//TEST FUNCTIONS - delete later!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void    	tokenization_testing(t_token_node *token_lst, t_single_linked_node *env);
void    printing_struct_content(t_cmd_data *cmd_data);
//static int	heredoc_filename_creation(char *filename, char *input, t_token_iteri *iteri);

#endif
