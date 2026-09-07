/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_err_msg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nilsdruon <nilsdruon@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 12:37:36 by nildruon          #+#    #+#             */
/*   Updated: 2026/09/05 13:42:00 by nilsdruon        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*add_to_string(char	*curr, char	*to_append1, char	*to_append2)
{
	char	*ret;

	if (curr)
		ret = ft_strjoin_three(curr, to_append1, to_append2);
	else
		ret = ft_strdup(to_append1);
	free(curr);
	return (ret);
}

void	err_msg(char	*func, char *value, char	*custom_err)
{
	char	*whole_msg;

	whole_msg = add_to_string(NULL, "minishell: ", NULL);
	if (!whole_msg)
		return ;
	if (func)
	{
		whole_msg = add_to_string(whole_msg, func, ": ");
		if (!whole_msg)
			return ;
	}
	if (value)
	{
		whole_msg = add_to_string(whole_msg, value, ": ");
	}
	if (!custom_err)
		whole_msg = add_to_string(whole_msg, strerror(errno), "");
	else
		whole_msg = add_to_string(whole_msg, custom_err, "");
	if (!whole_msg)
		return ;
	ft_putendl_fd(whole_msg, 2);
	free(whole_msg);
}
