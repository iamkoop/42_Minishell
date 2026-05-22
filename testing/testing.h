#ifndef TESTING_H
# define TESTING_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../minishell.h"
int main(int argc, char	**argv, char	**envp);
void testing_exec(char	**argv, char	**envp);
int env_tests(char **envp);

# endif