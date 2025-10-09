/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:40:30 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/09 15:05:21 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipeline(t_sh *sh)
{
	t_pipeline	pl;

	if (setup_cmds(&pl, sh) == -1)
		return (-1);
	while (pl.iter <= pl.count)
	{
		pl.current = &pl.cmds[pl.iter];
		if (own_cmd_exec(&pl, sh) == -1)
			return (pl_cleanup(&pl, sh, FAILURE));
		if (pipe_fork(&pl) == -1)
			return (pl_cleanup(&pl, sh, FAILURE));
		if (pl.current->pid == 0)
			child_process(&pl, sh);
		else if (parent_process(&pl) == -1)
			return (pl_cleanup(&pl, sh, FAILURE));
		pl.iter++;
	}
	return (pl_cleanup(&pl, sh, SUCCESS));
}
