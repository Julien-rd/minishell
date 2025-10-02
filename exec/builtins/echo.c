/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 19:00:30 by jromann           #+#    #+#             */
/*   Updated: 2025/10/02 11:29:22 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo(t_sh *sh, t_cmd *cmd, int nflag)
{
	size_t	iter;

	iter = 1;
	if (nflag == -1)
		return ;
	iter += nflag;
	while (cmd->cmd[iter])
	{
		if (safe_write(1, cmd->cmd[iter], ft_strlen(cmd->cmd[iter])) == -1)
			child_exit_handle(sh, cmd, 1);
		if (cmd->cmd[iter + 1])
		{
			if (safe_write(1, " ", 1) == -1)
				child_exit_handle(sh, cmd, 1);
		}
		iter++;
	}
	if (nflag == 0)
	{
		if (safe_write(1, "\n", 1) == -1)
			child_exit_handle(sh, cmd, 1);
	}
	child_exit_handle(sh, cmd, 0);
}