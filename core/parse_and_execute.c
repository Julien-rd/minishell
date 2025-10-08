/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_execute.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 10:51:19 by jromann           #+#    #+#             */
/*   Updated: 2025/10/08 14:43:04 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_central(t_sh *sh)
{
	int	exit_code;

	sh->exit = 0;
	exit_code = pipeline(sh);
	free_list(sh->entries);
	if (sh->heredoc)
		free2d(&sh->heredoc);
	return (exit_code);
}

int	parse_and_execute(char *buf, t_sh *sh, int flag)
{
	if (parsing(buf, sh) == -1)
		return (-1);
	if (syntax_check(sh) == -1)
		return (free_list(sh->entries), free2d(&sh->heredoc), sh->exit_code = 2,
			0);
	sh->exit_code = exec_central(sh);
	if (sh->exit_code == -1 && g_current_signal == 0)
		return (-1);
	if (sh->exit)
		return (sh->exit_code);
	if (flag == INTERACTIVE)
		add_history(buf);
	return (0);
}
