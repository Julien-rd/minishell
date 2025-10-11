/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 10:08:15 by jromann           #+#    #+#             */
/*   Updated: 2025/10/11 12:39:31 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_prompt(int num)
{
	(void)num;
	g_current_signal = SIGINT;
	if (safe_write(1, "\n", 1) == -1)
		return ;
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sigint_main(int num)
{
	(void)num;
	g_current_signal = SIGINT;
	if (safe_write(1, "\n", 1) == -1)
		return ;
}

void	sigint_heredoc(int num)
{
	(void)num;
	g_current_signal = SIGINT;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_replace_line("", 0);
	rl_on_new_line();
}

int	hdoc_signal_kill(char *buf, char *entry)
{
	if (buf)
		free(buf);
	if (g_current_signal != 2)
	{
		write(2, "warning: here-document delimited by end-of-file (wanted '",
			57);
		write(2, entry, ft_strlen(entry));
		write(2, "')\n", 3);
		return (0);
	}
	return (-1);
}
