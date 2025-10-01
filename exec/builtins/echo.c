/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 19:00:30 by jromann           #+#    #+#             */
/*   Updated: 2025/10/01 19:01:03 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo(t_exec *data, t_cmd *cmd, int nflag)
{
	size_t	iter;

	iter = 1;
	if (nflag == -1)
		return ;
	iter += nflag;
	while (cmd->cmd[iter])
	{
		if (safe_write(1, cmd->cmd[iter], ft_strlen(cmd->cmd[iter])) == -1)
			child_exit_handle(data, cmd, 1);
		if (cmd->cmd[iter + 1])
		{
			if (safe_write(1, " ", 1) == -1)
				child_exit_handle(data, cmd, 1);
		}
		iter++;
	}
	if (nflag == 0)
	{
		if (safe_write(1, "\n", 1) == -1)
			child_exit_handle(data, cmd, 1);
	}
	child_exit_handle(data, cmd, 0);
}