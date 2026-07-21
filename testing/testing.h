#ifndef TESTING_H
# define TESTING_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../minishell.h"
#include "../42_Libft/libft.h"

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
void    testing_parsing(t_single_linked_node *env);
void	tokenization_testing(t_token_node *token_lst, t_single_linked_node *env);
void	initiate_tokenization(char *input, t_single_linked_node *env);
int		heredoc_filename_creation(char *filename, char *input, t_token_iteri *iteri);
void	tokenization_unit_testing(t_single_linked_node *env);

# endif
