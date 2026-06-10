/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 15:38:17 by bastalze          #+#    #+#             */
/*   Updated: 2026/06/10 11:02:22 by bastalze         ###   ########.fr       */
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
//parsing
void	get_commandline_input(char **env);
int		tokenization(char *input, char **env, t_token_node *token_lst,
			t_token_iteri *iteri);
void	add_to_token(char c, t_token_node *token_lst, t_token_iteri *iteri);
void	delimit_token(char *input, char **env, t_token_node *token_lst,
				t_token_iteri *iteri);
char	*quote_removal(char *delimiter);
int		here_doc(char *input, char **env, t_token_node *token_lst,
			t_token_iteri *iteri);
void	error(char *message);
void	delete_hd_files();

//TEST FUNCTIONS - delete later!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void    	tokenization_testing(t_token_node *token_lst, char **env);
//static int	heredoc_filename_creation(char *filename, char *input, t_token_iteri *iteri);

#endif
