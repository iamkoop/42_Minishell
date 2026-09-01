/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 15:43:39 by bastalze          #+#    #+#             */
/*   Updated: 2026/09/01 16:53:13 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int		ft_get_commandline_input(t_single_linked_node *env, t_minishell *mini);
static void	initiate_tokenization(char *input, t_single_linked_node **env, t_minishell *mini);
void		reset_mini(t_minishell *mini);

int	get_commandline_input(t_single_linked_node *env, t_minishell *mini)
{
	char	*input;

	while (42)
	{
		if(mini->exe_exit)
		{
			ft_single_lstclear(&env, del_env_node_content);
			rl_clear_history();
			exit(mini->exit_status);
		}
		input = readline("Minishell> ");
		if (g_signal == SIGINT)
		{
			mini->exit_status = 130;
			g_signal = 0;
            //free(input);
			//continue ;
		}
		if (!input)
		{
			if (isatty(STDIN_FILENO))
				write(2, "exit\n", 5);
			return (0);
		}
		else if (input[0])
		{
			add_history(input);
			initiate_tokenization(input, &env, mini);
			free(input);
//			delete_hd_files();
		}
	}
}

static void	initiate_tokenization(char *input, t_single_linked_node **env,
			t_minishell *mini)
{
	t_token_iteri	iteri;

	ft_bzero(&iteri, sizeof(t_token_iteri));
	reset_mini(mini);
	arena_init_all(mini);
	start_first_token(mini, &iteri);
	tokenization(input, env, mini, &iteri);
}

void	reset_mini(t_minishell *mini)
{
	ft_bzero(mini->arena_tokens.data, ARENA_SIZE);
	ft_bzero(mini->arena_strings.data, ARENA_SIZE);
	mini->cmd_lst = NULL;
	mini->curr_cmd = NULL;
	mini->cmd_lst_size = -42;
	mini->next_pipe_fds[0] = -42;
	mini->next_pipe_fds[1] = -42;
	mini->prev_read_fd = -42;
	mini->redir_in = -42;
	mini->redir_out = -42;
	mini->in = -42;
	mini->out = -42;
	mini->builtin_has_been_redir = -42;
	mini->prev_in = -42;
	mini->prev_out = -42;
}
