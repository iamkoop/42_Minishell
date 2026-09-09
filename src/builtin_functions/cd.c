/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 01:23:00 by username          #+#    #+#             */
/*   Updated: 2026/09/09 19:40:32 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd(char **input, t_single_linked_node *envp)
{
	char		*target;
	t_pwds_vars	pwds;

	if (input[0] && input[1] && input[2])
		return (err_msg("cd", NULL, "too many arguments"), 1);
	if (input[1] && ft_strlen(input[1]) == 0)
		return (0);
	target = find_target(input, envp);
	if (!target)
		return (1);
	if (!getcwd(pwds.old_pwd, sizeof(pwds.old_pwd)))
		copy_pwd_from_env(&pwds, "PWD", envp);
	if (chdir(target) == -1)
		return (err_msg("cd", target, NULL), 1);
	if (!getcwd(pwds.new_pwd, sizeof(pwds.new_pwd)))
		return (above_dir_del_case(&pwds, envp, input[1]));
	if (input[1] && !ft_strcmp(input[1], "-"))
		printf("%s\n", pwds.new_pwd);
	if (!update_env(pwds.old_pwd, "OLDPWD", envp))
		err_msg("cd", NULL, "malloc fail in update_env");
	if (!update_env(pwds.new_pwd, "PWD", envp))
		err_msg("cd", NULL, "malloc fail in update_env");
	return (0);
}
