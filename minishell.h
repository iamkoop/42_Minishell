/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 15:38:17 by bastalze          #+#    #+#             */
/*   Updated: 2026/07/30 15:06:44 by nildruon         ###   ########.fr       */
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
	int		token;
	int		t;
	int		i;
}		t_token_iteri;

//Commands for execution:
typedef enum e_redir_type
{
	IN,
	OUT,
	APPEND,
	HERE,
} 		t_redir_type;

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

//vars structs that will need to goo
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

typedef struct s_print_sorted_env_vars
{
	t_single_linked_node    *tmp_lst;
    t_env_var               *tmp_env_var;
    t_single_linked_node    *curr_smllst_alpha;
	t_single_linked_node	*prev_smllst_alpha;
	int						lst_len;
	int						curr_cnt;
}				t_print_sorted_env_vars;

typedef struct s_export_vars
{
	t_single_linked_node    *node;

}				t_export_vars;

typedef struct s_exit_status
{
	int exit_status;

}				t_exit_status;


typedef struct s_minishell
{
	t_single_linked_node	*cmd_lst;
	t_command				*curr_cmd;
	int						exit_status;
	int						cmd_lst_size;
	int						next_pipe_fds[2];
	int						prev_read_fd;
	int						redir_in;
	int						redir_out;
	int						in;
	int						out;
	int						builtin_has_been_redir; // when redirectiing a builtin the programm should redirect the it back to in/out especially when not in a child process
	int						prev_in; //initialze to -42 ! cuz 0 could be a valid fd and -1 is error num
	int						prev_out; //initialze to -42 ! cuz 0 could be a valid fd and -1 is error num
}				t_minishell;


//environment stuff
t_single_linked_node	*env_to_lst(char	**envp);
void					del_env_node_content(void	*content);
char					**env_to_char_arr(t_single_linked_node	*lst);
t_single_linked_node	*get_env_from_lst(char	*to_find, t_single_linked_node	*envp);

//builtins
int						env(char	**input, t_single_linked_node	*envp);
int						echo(char	**input);
void					builtin_exit(char	**input);
int						pwd(char	**input);
int						cd(char **input, t_single_linked_node	*envp);
int						unset(char	**input, t_single_linked_node	**envp);
int						export(char **input, t_single_linked_node *envp);

int						is_builtin(char *cmd);
int						exec_command(char   **cmd_and_args, t_single_linked_node    *envp);
char					*get_path(char *cmd, t_single_linked_node   *envp, t_exit_status *mini);

int						redirections(t_single_linked_node	*redir_lst, t_minishell	*mini);
void					child_process(t_minishell *mini, t_single_linked_node	*envp, int close_read, int child_type);
void					parent(t_minishell *mini, t_single_linked_node	*envp);
//Functions of minishell:
void					get_commandline_input(char **env);
int						tokenization(char *input, char **env, t_token_node *token_lst,
			t_token_iteri *iteri);
void					add_to_token(char c, t_token_node *token_lst, t_token_iteri *iteri);
void					delimit_token(char *input, char **env, t_token_node *token_lst,
				t_token_iteri *iteri);
char					*quote_removal(char *delimiter);
int						here_doc(char *input, char **env, t_token_node *token_lst,
			t_token_iteri *iteri);

//Testers:
void					tokenization_testing(t_token_node *token_lst, char **env);
void					initiate_tokenization(char *input, char **env);
int						heredoc_filename_creation(char *filename);

