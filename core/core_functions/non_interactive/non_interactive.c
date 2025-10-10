/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_interactive.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:46:45 by jromann           #+#    #+#             */
/*   Updated: 2025/10/10 10:06:34 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cut_nl(char *buf)
{
	size_t	len;

	len = ft_strlen(buf);
	if (len == 0)
		return ;
	buf[len - 1] = '\0';
}

void	non_interactive(t_sh *sh)
{
	setup_noninteractive_signals();
	while (1)
	{
		sh->buf = get_next_line(STDIN_FILENO);
		if (!sh->buf)
			break ;
		cut_nl(sh->buf);
		if (empty_prompt(sh->buf))
			continue ;
		parse_and_execute(sh);
		if (sh->exit_code == -1 || sh->exit)
		{
			get_next_line(-1);
			check_exit_status(sh);
		}
		free(sh->buf);
	}
	get_next_line(-1);
	check_exit_status(sh);
}
