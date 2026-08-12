/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_heredocs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 15:51:51 by bastalze          #+#    #+#             */
/*   Updated: 2026/06/04 18:20:50 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../minishell.h"

void		delete_hd_files();
static void	heredoc_filename_generator(char *filenamei, int *num);

void	delete_hd_files()
{
	char	filename[16];
	static int	num;

	while (42)
	{
		heredoc_filename_generator(filename, &num);
		if (access(filename, F_OK))
			break ;
		unlink(filename);
	}
	num = 0;
}

static void	heredoc_filename_generator(char *filename, int *num)
{
	char		*start;

	start = ".hd/File_";
	memcpy(filename, start, 9);
	filename[9] = *num / 10 + '0';
	filename[10] = *num % 10 + '0';
	filename[11] = '.';
	filename[12] = 't';
	filename[13] = 'x';
	filename[14] = 't';
	filename[15] = 0;
	(*num)++;
}
