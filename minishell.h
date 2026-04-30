#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "libft_lib/libft.h"
# include <errno.h>

//command execution
void execute_command(char	*cmd_and_args);

#endif