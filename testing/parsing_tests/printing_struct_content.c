/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing_struct_content.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bastalze <bastalze@student.42vienna.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 14:31:41 by bastalze          #+#    #+#             */
/*   Updated: 2026/07/24 17:36:35 by bastalze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../minishell.h"
#include <stdio.h>

void	printing_struct_content(t_cmd_data *cmd_data)
{
	t_single_linked_node	*curr_cmd_node;
	t_command				*tmp_cmd;
	t_single_linked_node	*curr_redir;
	t_redir_list			*tmp_redir_struct;
	const char				*redir_types[] = {"IN", "OUT", "APPEND", "HERE"};
	int 					i = 0;
	int						j = 0;

	curr_cmd_node = cmd_data->head;
	printf("====================================================\n");
	printf("       PRINTING STRUCT CONTENT\n");
	printf("====================================================\n");
	while(curr_cmd_node)
	{
		printf("COMMAND NUMBER %d\n", j);
		printf("----------------------------------------------------\n");
		tmp_cmd = (t_command *)curr_cmd_node->content;
		if(tmp_cmd->argv)
		{
			i = 0;
			printf("ARGV WORDS\n");
			if(tmp_cmd->argv)
			while(tmp_cmd->argv[i])
			{
				printf("- argv word %d: %s\n", i, tmp_cmd->argv[i]);
				i++;
			}
			printf("\n");
		}
		if(tmp_cmd->redir)
		{
			curr_redir = tmp_cmd->redir;
			printf("REDIRECTIONS\n");
			i = 1;
			while(curr_redir)
			{
				tmp_redir_struct = (t_redir_list *)curr_redir->content;
				printf("Redirection %d\n", i);
				printf("- Redirection type: %s\n", redir_types[tmp_redir_struct->redir_type]);
				printf("- Filename: %s\n", tmp_redir_struct->filename);
				printf("- fd: %d", tmp_redir_struct->fd);
				if (tmp_redir_struct->fd)
					close (tmp_redir_struct->fd);
				curr_redir = curr_redir->next;
				i++;
				printf("\n");
			}
		}
		printf("\n");
		curr_cmd_node = curr_cmd_node->next;
		j++;
	}
}
