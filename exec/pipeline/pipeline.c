/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:40:30 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/03 14:59:09 by eprottun         ###   ########.fr       */
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
		own_cmd_exec(&pl, sh);
		if (pipe_fork(&pl) == -1)
			return (-1);
		if (pl.current->pid == 0)
			child_process(&pl, sh);
		else
			if (parent_process(&pl, sh) == -1)
				return (-1);
		pl.iter++;
	}
	return (kill_children(&pl, sh));
}
