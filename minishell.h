/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 15:38:17 by bastalze          #+#    #+#             */
/*   Updated: 2026/09/01 16:41:30 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef MINISHELL_H
# define MINISHELL_H
# define _GNU_SOURCE
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

# define TOKEN_AMOUNT 100
# define TOKEN_STR_SIZE 5000
# define HD_DELIMITER_LEN 50 
# define WORD_AMOUNT 100
# define WORD_STR_SIZE 1000
# define ARENA_SIZE 40096

extern volatile sig_atomic_t g_signal;

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

typedef struct s_arena
{
	char	data[ARENA_SIZE];
	size_t	cap;
	size_t	pos;
}	t_arena;

typedef struct s_token_node
{
	enum e_token_type	token_type;
	char				*token_str;
}		t_token_node;

typedef struct s_token_iteri
{
	t_token_node	*tok;
	int				str_pos;
	int				i;
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
	char	**field;
	int		str_pos;
	int		split_count;
	bool	has_char;
	bool	quoted;
	bool	heredoc;
	int		exit_status;
	size_t	word_count;
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
	t_arena					arena_strings;
	t_arena					arena_tokens;
	t_arena					arena_split_strings;
	t_arena					arena_split_tokens;
	int						heredoc_write_fd;
	bool					quote_mode_entered;
	t_single_linked_node	*cmd_lst;
	t_command				*curr_cmd; //initialized to NULL every new commandline input, same for the -42s below
	int						exit_status;
	bool					exe_exit;
	int						cmd_lst_size; //initialzed to -42
	int						next_pipe_fds[2]; //initialzed to -42
	int						prev_read_fd; //initialzed to -42
	int						redir_in; //initialzed to -42
	int						redir_out; //initialzed to -42
	int						in;//initialzed to -42
	int						out;//initialzed to -42
	int						builtin_has_been_redir; // when redirectiing a builtin the programm should redirect the it back to in/out especially when not in a child process
	int						prev_in; //initialze to -42 ! cuz 0 could be a valid fd and -1 is error num
	int						prev_out; //initialze to -42 ! cuz 0 could be a valid fd and -1 is error num
}				t_minishell;

//beginning of minishell
int						initializing_minishell(char **envp);
void					get_commandline_input(t_single_linked_node **env, t_minishell *mini);

//environment stuff
t_single_linked_node	*env_to_lst(char	**envp);
void					del_env_node_content(void	*content);
char					**env_to_char_arr(t_single_linked_node	*lst);
t_single_linked_node	*get_env_from_lst(char	*to_find, t_single_linked_node	*envp);
void					free_env_lst(t_single_linked_node *env);
t_env_var				*create_env_node(char   *str);
t_single_linked_node	*default_env();

//builtins
int						env(char	**input, t_single_linked_node	**envp);
int						echo(char	**input);
int						builtin_exit(char	**input, t_minishell	*mini);
int						pwd(char	**input);
int						cd(char **input, t_single_linked_node	*envp);
int						unset(char	**input, t_single_linked_node	**envp);
int						export(char **input, t_single_linked_node **envp);

int						is_builtin(char *cmd);
void						exec_command(char   **cmd_and_args, t_single_linked_node	**envp, t_minishell *mini);
char					*get_path(char *cmd, t_single_linked_node   *envp, t_minishell *mini);
void					exec_main(t_minishell *mini, t_single_linked_node	*cmd_lst, t_single_linked_node	**envp);
int						builtin_redir_special_case(t_minishell	*mini, t_single_linked_node	**envp);

int						exec_redirections(t_single_linked_node	*redir_lst, t_minishell	*mini);
void					child_process(t_minishell *mini, t_single_linked_node	**envp, int close_read, int child_type);
void					parent(t_minishell *mini, t_single_linked_node	**envp);

void					err_msg(char	*func, char *value, char	*custom_err);

//PARSING PART
//tokenization
int		tokenization(char *input, t_single_linked_node **env, t_minishell *mini,
			t_token_iteri *iteri);
int		here_or_append(char *input, t_single_linked_node *env,
			t_minishell *mini, t_token_iteri *iteri);
int		operators1(char *input, t_single_linked_node *env,
			t_minishell *mini, t_token_iteri *iteri);
int		operators2(char *input, t_single_linked_node *env,
			t_minishell *mini, t_token_iteri *iteri);
int		redirections(char *input, t_single_linked_node *env,
			t_minishell *mini, t_token_iteri *iteri);
int		start_first_token(t_minishell *mini, t_token_iteri *iteri);
int		add_to_token(char c, t_minishell *mini, t_token_iteri *iteri);
int		delimit_token(t_minishell *mini, t_token_iteri *iteri);

//here_doc
char	*quote_removal(char *delimiter);
int		here_doc(t_single_linked_node *env, t_minishell *mini,
			t_redir_list *redir_content);
int		adding_heredoc_into_file(t_minishell *mini, bool expansion, char *delimiter,
			t_single_linked_node *env);
int		check_for_heredoc(t_minishell *mini, t_cmd_data *cmd_data,
        t_single_linked_node **env);

//error and exit functions
void	error(char *message);
void    free_command_struct(t_single_linked_node *cmd_lst);
void	close_fd(int	*fd);
void	close_all_fds(t_minishell  *mini);
void    free_all(t_single_linked_node *env, t_minishell *mini);
void    close_heredoc_fds(t_minishell *mini);

// parsing
int		initiate_parsing(t_single_linked_node **env, t_minishell *mini,
			t_token_iteri *iteri);
int		parsing(t_single_linked_node **env, t_minishell *mini, t_token_iteri *iteri,
			t_cmd_data *cmd_data);
int		is_redirection(t_arena *arena_tokens, t_token_iteri *iteri);
int		redirect(t_single_linked_node *env,
			t_token_iteri *iteri, t_cmd_data *cmd_data,
			t_minishell *mini);
int		add_word_to_struct(t_cmd_data *cmd_data,
				t_minishell *mini);
void	free_strarray(char **array);
size_t	ft_2darraylen(char **word);
size_t	ft_strarraylen(char **argv);

//quote removal and variable expansion
int		quote_rm_var_expan(char *s, t_minishell *mini,
			t_single_linked_node *env, t_quote_iteri *iteri);
int		dollar_found(char *s, t_minishell *mini,
			t_quote_iteri *iteri, t_single_linked_node *env);
int		find_var(char *var, t_minishell *mini,
			t_quote_iteri *iteri, t_single_linked_node *env);
void 	init_qrve_arena(t_minishell *mini);
int		start_first_word(t_minishell *mini, t_quote_iteri *iteri);
int		add_to_word(char c, t_minishell *mini, t_quote_iteri *iteri);
int		delimit_word_array(t_minishell *mini, t_quote_iteri *iteri);

//TEST FUNCTIONS - delete later!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void    tokenization_testing(t_token_node *token_lst, t_single_linked_node *env);
void    printing_struct_content(t_cmd_data *cmd_data);
void    main_testing(char **argv, char **env);
void    testing_parsing(t_single_linked_node *env);

//static int	heredoc_filename_creation(char *filename, char *input, t_token_iteri *iteri);

void	main_testing(char **argv, char **envp);

//arenas
t_arena	arena_init(void);
void	*get_arena_element_start(t_arena *arena);
bool	grow_arena_element(t_arena *arena, size_t size);
void	arena_init_all(t_minishell *mini);


#endif
