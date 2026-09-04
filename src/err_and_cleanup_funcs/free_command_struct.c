/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_command_struct.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 12:52:45 by bastalze          #+#    #+#             */
/*   Updated: 2026/08/20 21:34:15 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void		free_command_struct(t_single_linked_node *cmd_lst);
static void	free_argv(t_command *cmd_content);
static void	free_redir(t_command *cmd_content);
static void	free_redir_content(t_redir_list *redirection_content);

void	free_command_struct(t_single_linked_node *cmd_lst)
{
	t_single_linked_node	*curr_cmd_node;
	t_single_linked_node	*next_cmd_node;
	t_command				*cmd_content;

	curr_cmd_node = cmd_lst;
	while (curr_cmd_node)
	{
		cmd_content = (t_command *)curr_cmd_node->content;
		if (cmd_content)
		{
			free_argv(cmd_content);
			free_redir(cmd_content);
			free(cmd_content);
			cmd_content = NULL;
		}
		next_cmd_node = curr_cmd_node->next;
		free(curr_cmd_node);
		curr_cmd_node = next_cmd_node;
	}
	cmd_lst = NULL;
}

static void	free_argv(t_command *cmd_content)
{
	int	i;

	if (cmd_content->argv)
	{
		i = 0;
		while (cmd_content->argv[i])
		{
			free(cmd_content->argv[i]);
			cmd_content->argv[i] = NULL;
			i++;
		}
		free(cmd_content->argv);
		cmd_content->argv = NULL;
	}
}

static void	free_redir(t_command *cmd_content)
{
	t_single_linked_node	*curr_redir_node;
	t_single_linked_node	*next_redir_node;
	t_redir_list			*redirection_content;

	curr_redir_node = cmd_content->redir;
	while (curr_redir_node)
	{
		if (curr_redir_node->content)
		{
			redirection_content = (t_redir_list *)curr_redir_node->content;
			free_redir_content(redirection_content);
			free(redirection_content);
			redirection_content = NULL;
		}
		next_redir_node = curr_redir_node->next;
		free(curr_redir_node);
		curr_redir_node = next_redir_node;
	}
}

static void	free_redir_content(t_redir_list *redirection_content)
{
	if (redirection_content->filename)
	{
		free(redirection_content->filename);
		redirection_content->filename = NULL;
	}
	if (redirection_content->fd > -1)
	{
		close(redirection_content->fd);
		redirection_content->fd = -42;
	}
}
