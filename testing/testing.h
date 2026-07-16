#ifndef TESTING_H
# define TESTING_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "../minishell.h"

//main
int main(int argc, char	**argv, char	**envp);

//testing all execution
void testing_exec(char	**argv, char	**envp);

//environment conversion tests
int env_tests(char **envp);
int env_manipulation_tests(char **envp);

//builtins testing
int builtins_tests(char **argv, char **envp);
int test_env(char **envp);
int	test_echo();
int test_builtin_exit(char **envp);
int test_pwd(void);
int test_cd(char **envp);
int test_unset(char **envp);
int test_export(char **envp);
int test_exec_command(char **envp);
int test_redirections_suite(void);

# endif