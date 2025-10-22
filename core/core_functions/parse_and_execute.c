/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_execute.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 10:51:19 by jromann           #+#    #+#             */
/*   Updated: 2025/10/22 13:42:58 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cleanup(t_sh *sh)
{
	free_list(sh->entries);
	close_fd(sh);
	free(sh->heredoc_fd);
}

static int	exec_central(t_sh *sh)
{
	int	exit_code;

	sh->exit = 0;
	exit_code = pipeline(sh);
	return (exit_code);
}

void	parse_and_execute(char *buf, t_sh *sh)
{
	if (parsing(buf, sh) == -1)
		return (free(buf));
	free(buf);
	if (syntax_check(sh) == -1)
		return (cleanup(sh));
	sh->exit_code = exec_central(sh);
	return (cleanup(sh));
}
