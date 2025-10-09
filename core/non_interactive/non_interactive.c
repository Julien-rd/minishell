/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_interactive.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:46:45 by jromann           #+#    #+#             */
/*   Updated: 2025/10/09 15:25:16 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
