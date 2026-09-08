/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 15:38:17 by bastalze          #+#    #+#             */
/*   Updated: 2026/09/08 11:45:06 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
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
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
//Delete after testing!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# include <assert.h>

# define HD_DELIMITER_LEN 100
# define VAR_SIZE 1024
# define ARENA_SIZE 65536 // 64 KB

extern volatile sig_atomic_t	g_signal;
// sig_atomic_t: An integer type that can be accessed as an atomic entity: the
// computer can read or write this variable in a single, uninterruptible
// machine instruction
// extern: Variable is declared here and defined 
// volatile: without it the value of the variable might be chached, with it
// each time the variable is used the actual memory is being read

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

typedef struct s_cmd_data
{
	t_single_linked_node	*head;
	t_single_linked_node	*tail;
}		t_cmd_data;

//Heredoc
typedef struct s_heredoc_data
{
	char	*heredoc_input;
	char	*eof_input;
	bool	eof_nonempty_line;
	bool	expansion;
}		t_heredoc_data;

//Adding word to command struct
typedef struct s_word_iteri
{
	int		i;
	int		j;
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
	t_single_linked_node	*tmp_lst;
	t_env_var				*tmp_env_var;
	t_single_linked_node	*curr_smllst_alpha;
	t_single_linked_node	*prev_smllst_alpha;
	int						lst_len;
	int						curr_cnt;
}				t_print_sorted_env_vars;

typedef struct s_export_vars
{
	t_single_linked_node	*node;

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
	t_single_linked_node	*cmd_lst_iteri;
	t_command				*curr_cmd;
	int						exit_status;
	bool					exe_exit;
	int						cmd_lst_size;
	int						next_pipe_fds[2];
	int						prev_read_fd;
	int						redir_in;
	int						redir_out;
	int						in;
	int						out;
	int						builtin_has_been_redir;
	int						prev_in;
	int						prev_out;
}				t_minishell;

//beginning of minishell
int						initializing_minishell(char **envp);
void					get_commandline_input(t_single_linked_node **env,
							t_minishell *mini);
void					set_sigquit_to_default(void);
void					set_sigint_to_default(void);
void					ignore_sigquit(void);
void					signal_strl_c(void);
void					ignore_sigint(void);

//environment stuff
t_single_linked_node	*env_to_lst(char	**envp);
void					del_env_node_content(void	*content);
char					**env_to_char_arr(t_single_linked_node	*lst);
t_single_linked_node	*get_env_from_lst(char	*to_find,
							t_single_linked_node	*envp);
void					free_env_lst(t_single_linked_node *env);
t_env_var				*create_env_node(char	*str);
t_single_linked_node	*default_env(void);

//builtins
int						env(char	**input, t_single_linked_node	**envp);
int						echo(char	**input);
int						builtin_exit(char	**input, t_minishell	*mini);
int						pwd(char	**input);
int						cd(char **input, t_single_linked_node	*envp);
int						unset(char	**input, t_single_linked_node	**envp);
int						export(char **input, t_single_linked_node **envp);

int						is_builtin(char *cmd);
void					exec_command(char	**cmd_and_args,
							t_single_linked_node	**envp, t_minishell *mini);
char					*get_path(char *cmd, t_single_linked_node	*envp,
							t_minishell *mini);
void					exec_main(t_minishell *mini,
							t_single_linked_node	*cmd_lst,
							t_single_linked_node	**envp);
int						builtin_redir_special_case(t_minishell	*mini,
							t_single_linked_node	**envp);

int						exec_redirections(t_single_linked_node	*redir_lst,
							t_minishell	*mini);
void					child_process(t_minishell *mini,
							t_single_linked_node	**envp, int close_read,
							int child_type);
void					parent(t_minishell *mini, t_single_linked_node	**envp);

void					err_msg(char	*func, char *value,
							char	*custom_err);

//PARSING PART
//tokenization
int						tokenization(char *input, t_single_linked_node **env, t_minishell *mini,
							t_token_iteri *iteri);
int						here_or_append(char *input, t_single_linked_node *env,
							t_minishell *mini, t_token_iteri *iteri);
int						operators1(char *input, t_single_linked_node *env,
							t_minishell *mini, t_token_iteri *iteri);
int						operators2(char *input, t_single_linked_node *env,
							t_minishell *mini, t_token_iteri *iteri);
int						redirections(char *input, t_single_linked_node *env,
							t_minishell *mini, t_token_iteri *iteri);
int						start_first_token(t_minishell *mini, t_token_iteri *iteri);
int						add_to_token(char c, t_minishell *mini, t_token_iteri *iteri);
int						delimit_token(t_minishell *mini, t_token_iteri *iteri);

//here_doc
char					*quote_removal(char *delimiter);
int						here_doc(t_single_linked_node *env, t_minishell *mini,
							t_redir_list *redir_content);
int						adding_heredoc_into_file(t_minishell *mini, bool expansion, char *delimiter,
							t_single_linked_node *env);
int						expand_n_write(t_heredoc_data *hd_data,
							t_minishell *mini, t_single_linked_node *env);
int						check_for_heredoc(t_minishell *mini, t_cmd_data *cmd_data,
        					t_single_linked_node **env);

//error and exit functions
void					error(char *message);
void    				free_command_struct(t_single_linked_node *cmd_lst);
void					close_fd(int	*fd);
void					close_all_fds(t_minishell  *mini);
void    				free_all(t_single_linked_node *env, t_minishell *mini);
void    				close_heredoc_fds(t_minishell *mini);

// parsing
int						initiate_parsing(t_single_linked_node **env,
							t_minishell *mini, t_token_iteri *iteri);
int						parsing(t_single_linked_node **env, t_minishell *mini,
							t_token_iteri *iteri, t_cmd_data *cmd_data);
int						is_redirection(t_arena *arena_tokens,
							t_token_iteri *iteri);
int						redirect(t_single_linked_node *env,
							t_token_iteri *iteri, t_cmd_data *cmd_data,
							t_minishell *mini);
int						add_word_to_struct(t_cmd_data *cmd_data,
							t_minishell *mini);
void					free_strarray(char **array);
size_t					ft_2darraylen(char **word);
size_t					ft_strarraylen(char **argv);

//quote removal and variable expansion
int						quote_rm_var_expan(char *s, t_minishell *mini,
							t_single_linked_node *env, t_quote_iteri *iteri);
int						dollar_found(char *s, t_minishell *mini,
							t_quote_iteri *iteri, t_single_linked_node *env);
int						find_var(char *var, t_minishell *mini,
							t_quote_iteri *iteri, t_single_linked_node *env);
void					init_qrve_arena(t_minishell *mini);
int						start_first_word(t_minishell *mini,
							t_quote_iteri *iteri);
int						add_to_word(char c, t_minishell *mini,
							t_quote_iteri *iteri);
int						delimit_word_array(t_minishell *mini,
							t_quote_iteri *iteri);

//TEST FUNCTIONS - delete later!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void					tokenization_testing(t_token_node *token_lst,
							t_single_linked_node *env);
void					printing_struct_content(t_cmd_data *cmd_data);
void					main_testing(char **argv, char **env);
void					testing_parsing(t_single_linked_node *env);
void					main_testing(char **argv, char **envp);

//arenas
t_arena					arena_init(void);
void					*get_arena_element_start(t_arena *arena);
bool					grow_arena_element(t_arena *arena, size_t size);
void					arena_init_all(t_minishell *mini);

#endif
