/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_central.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 19:10:38 by jromann           #+#    #+#             */
/*   Updated: 2025/10/02 14:31:10 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_sh(t_sh *sh)
{
	// sh->pipe.count = 0;
	sh->exit = 0;
	// if (init_pipe_pos(sh) == -1)
	// 	return (free2d(&sh->heredoc), free2d(&sh->envp.vars), -1);
	return (0);
}

int	exec_central(t_sh *sh)
{
	int		exit_code;

	exit_code = 0;
	// return -1;
	if (init_sh(sh) == -1)
		return (free_list(sh->entries), -1);
	exit_code = pipeline(sh);
	free_list(sh->entries);
	if(sh->heredoc)
		free2d(&sh->heredoc);
	if (exit_code == -1)
		free2d(&sh->envp.vars);
	return (exit_code);
}
