/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:40:30 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/15 17:54:54 by eprottun         ###   ########.fr       */
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
			return (pl_cleanup(&pl, sh, FAILURE));
		if (pipe_fork(&pl) == -1)
			return (pl_cleanup(&pl, sh, FAILURE));
		if (pl.current->pid == 0)
			child_process(&pl, sh);
		else if (parent_process(&pl) == -1)
			return (pl_cleanup(&pl, sh, FAILURE));
		pl.iter++;
		pl.hdoc_iter += hdoc_add(&pl);
	}
	return (pl_cleanup(&pl, sh, SUCCESS));
}
