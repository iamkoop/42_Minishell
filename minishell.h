/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 15:38:17 by bastalze          #+#    #+#             */
/*   Updated: 2026/08/12 13:39:59 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef MINISHELL_H
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
# include <signal.h>

//Delete after testing!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#include <assert.h>

# define TOKEN_AMOUNT 10
# define TOKEN_STR_SIZE 50
# define HD_DELIMITER_LEN 50 
# define WORD_AMOUNT 40
# define WORD_STR_SIZE 50

extern volatile sig_atomic_t signal_code;

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

//Adding word to command struct
typedef struct s_word_iteri
{
	int 	i;
	int 	j;
	size_t	argv_i;
	size_t	argv_j;
}		t_word_iteri;

//Quote_removal_variable_expansion
typedef struct s_quote_iteri
{
	int		i;
	int		wi;
	int		wj;
	bool	quoted;
	bool	heredoc;
}		t_quote_iteri;

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
void					exec_command(char   **cmd_and_args, t_single_linked_node    *envp, t_minishell *mini);
char					*get_path(char *cmd, t_single_linked_node   *envp, t_minishell *mini);
void					exec_main(t_minishell *mini, t_single_linked_node	*cmd_lst, t_single_linked_node	*envp);
int						builtin_redir_special_case(t_minishell	*mini, t_single_linked_node	*envp);

int						exec_redirections(t_single_linked_node	*redir_lst, t_minishell	*mini);
void					child_process(t_minishell *mini, t_single_linked_node	*envp, int close_read, int child_type);
void					parent(t_minishell *mini, t_single_linked_node	*envp);

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
int		delimit_token(char *input, t_single_linked_node *env, t_token_node *token_lst,
				t_token_iteri *iteri);

//here_doc
char	*quote_removal(char *delimiter);
int		here_doc(char *input, t_single_linked_node *env, t_token_node *token_lst,
			t_token_iteri *iteri);
int		adding_heredoc_into_file(int fd, bool expansion, char *delimiter,
				t_single_linked_node *env);
//error and exit functions
void	error(char *message);
void	delete_hd_files();
void    free_command_struct(t_cmd_data *cmd_data);
void	close_fd(int	*fd);

// parsing
int		initiate_parsing(t_single_linked_node *env, t_token_node *token_lst,
			t_token_iteri *iteri);
int		parsing(t_single_linked_node *env, t_token_node *token_lst, t_token_iteri *iteri,
			t_cmd_data *cmd_data);
int		is_redirection(t_token_node *token_lst, t_token_iteri *iteri);
int		redirect(t_single_linked_node *env, t_token_node *token_lst,
			t_token_iteri *iteri, t_cmd_data *cmd_data);
int		add_word_to_struct(t_cmd_data *cmd_data,
				char word[WORD_AMOUNT][WORD_STR_SIZE]);
void	free_strarray(char **array);
size_t	ft_2darraylen(char word[WORD_AMOUNT][WORD_STR_SIZE]);
size_t	ft_strarraylen(char **argv);

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
