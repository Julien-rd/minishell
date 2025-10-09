/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_interactive.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:46:45 by jromann           #+#    #+#             */
/*   Updated: 2025/10/09 16:30:35 by eprottun         ###   ########.fr       */
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
	char	*buf;

	setup_noninteractive_signals();
	while (1)
	{
		buf = get_next_line(STDIN_FILENO);
		if (!buf)
			break ;
		cut_nl(buf);
		if (empty_prompt(buf))
			continue ;
		parse_and_execute(buf, sh);
		if (sh->exit_code || sh->exit)
		{
			get_next_line(-1);
			check_exit_status(buf, sh);
		}
		free(buf);
	}
	check_exit_status(buf, sh);
}
