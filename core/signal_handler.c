/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 10:08:15 by jromann           #+#    #+#             */
/*   Updated: 2025/09/28 12:24:07 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_prompt(int num)
{
	g_current_signal = SIGINT;
	if (safe_write(1, "\n", 1) == -1)
		return ;
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sigint_main(int num)
{
	g_current_signal = SIGINT;
	if (safe_write(1, "\n", 1) == -1)
		return ;
}

void	sigint_heredoc(int num)
{
	g_current_signal = SIGINT;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_replace_line("", 0);
	rl_on_new_line();
}
