/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:40:30 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/09 15:08:12 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipeline(t_sh *sh)
{
	t_pipeline	pl;

	if (pl_setup(&pl, sh) == -1)
		return (-1);
	while (pl.iter <= pl.count)
	{
		pl.current = &pl.cmds[pl.iter];
		if (own_cmd_exec(&pl, sh) == -1)
			return (free_cmds(&pl, pl.count + 1), free(pl.position), -1);
		if (pipe_fork(&pl) == -1)
			return (free_cmds(&pl, pl.count + 1), free(pl.position), -1);
		if (pl.current->pid == 0)
			child_process(&pl, sh);
		else if (parent_process(&pl) == -1)
			return (free_cmds(&pl, pl.count + 1), free(pl.position), -1);
		pl.iter++;
	}
	return (free_cmds(&pl, pl.count + 1), free(pl.position), kill_children(&pl,
			sh));
}
