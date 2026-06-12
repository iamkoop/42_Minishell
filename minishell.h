/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 15:38:17 by bastalze          #+#    #+#             */
/*   Updated: 2026/06/12 16:34:41 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

//env_var_struct
typedef struct s_env_var
{
	char				*key;
	char				*value;
}					t_env_var;

typedef struct s_create_env_node_vars
{
	size_t			key_len;
	size_t			str_len;
	int				no_equals;
}					t_create_env_node_vars;


typedef struct s_pwds_vars
{
	char				old_pwd[4096];
	char				new_pwd[4096];
	int					is_dash;
}					t_pwds_vars;

typedef struct s_pwd_and_key_len
{
	size_t 		pwd_l;
	size_t		key_l;
}				t_pwd_and_key_len;

//environment stuff
t_single_linked_node	*env_to_lst(char	**envp);
void					del_env_node_content(void	*content);
char					**env_to_char_arr(t_single_linked_node	*lst);
t_single_linked_node	*get_env_from_lst(char	*to_find, t_single_linked_node	*envp);

//builtins
void					env(t_single_linked_node	*envp);
void					echo(char	**input);
void					builtin_exit(char	**input);
void					pwd(char	**input);
int						cd(char **input, t_single_linked_node	*envp);
int						unset(char	**input, t_single_linked_node	**envp);

//Functions of minishell:
void	get_commandline_input(char **env);
int		tokenization(char *input, char **env, t_token_node *token_lst,
			t_token_iteri *iteri);
void	add_to_token(char c, t_token_node *token_lst, t_token_iteri *iteri);
void	delimit_token(char *input, char **env, t_token_node *token_lst,
				t_token_iteri *iteri);
char	*quote_removal(char *delimiter);
int		here_doc(char *input, char **env, t_token_node *token_lst,
			t_token_iteri *iteri);

//Testers:
void	tokenization_testing(t_token_node *token_lst, char **env);
void	initiate_tokenization(char *input, char **env);
int		heredoc_filename_creation(char *filename);

