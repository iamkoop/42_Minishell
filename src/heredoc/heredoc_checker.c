/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_checker.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 10:56:18 by bastalze          #+#    #+#             */
/*   Updated: 2026/08/20 21:33:42 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int check_for_heredoc(t_minishell *mini, t_cmd_data *cmd_data,
        t_single_linked_node **env)
{
    t_single_linked_node    *cmd_node;
    t_command               *cmd_content;
    t_single_linked_node    *redir_node;
    t_redir_list            *redir_content;

    cmd_node = cmd_data->head;
    while (cmd_node)
    {
        cmd_content = (t_command *)cmd_node->content;
        redir_node = cmd_content->redir;
        while (redir_node)
        {
            redir_content = (t_redir_list *)redir_node->content;
            if (redir_content->redir_type == HERE)
            {
                if (here_doc(*env, mini, redir_content))
                    return (mini->exit_status = 1, 1);
            }
            redir_node = redir_node->next;
        }
        cmd_node = cmd_node->next;
    }
    return (0);
}
