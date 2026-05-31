#ifndef TESTING_H
# define TESTING_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../minishell.h"

//main
int main(int argc, char	**argv, char	**envp);

//testing all execution
void testing_exec(char	**argv, char	**envp);

//environment conversion tests
int env_tests(char **envp);

//builtins testing
int builtins_tests(char **argv, char **envp);
int test_env(char **envp);
int	test_echo();

//parsing testing:
void    testing_parsing(char **env);
void	tokenization_testing(t_token_node *token_lst, char **env);
void	initiate_tokenization(char *input, char **env);
int		heredoc_filename_creation(char *filename);

# endif
