/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_interactive.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:46:45 by jromann           #+#    #+#             */
/*   Updated: 2025/10/04 19:21:55 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	non_interactive(t_sh *sh)
{
	char	*buf;
	int		exit_code;

	setup_noninteractive_signals();
	while (1)
	{
		buf = get_next_line(STDIN_FILENO);
		if (!buf)
			break ;
		cut_nl(buf);
		if (empty_prompt(buf))
			continue ;
		exit_code = parse_and_execute(buf, sh, NONINTERACTIVE);
		if (exit_code || sh->exit)
			return (free(buf), free2d(&sh->envp.vars), get_next_line(-1),
				exit(exit_code));
		free(buf);
	}
	free2d(&sh->envp.vars);
	exit(sh->exit_code);
}
