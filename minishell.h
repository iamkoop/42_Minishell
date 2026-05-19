#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "42_Libft/libft.h"
# include <errno.h>

typedef struct s_env_var
{
	char				*key;
	char				*value;
}					t_env_var;

//command execution
void execute_command(char	*cmd_and_args);
t_single_linked_node	*env_lst(char	**envp);
# endif