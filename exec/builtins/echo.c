/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 19:00:30 by jromann           #+#    #+#             */
/*   Updated: 2025/10/07 17:24:29 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo(t_pipeline *pl, t_sh *sh, int nflag)
{
	size_t	iter;

	iter = 1;
	if (nflag == -1)
		return ;
	iter += nflag;
	while (pl->current->argv[iter])
	{
		if (safe_write(1, pl->current->argv[iter],
				ft_strlen(pl->current->argv[iter])) == -1)
			child_exit_handle(sh, pl, 1);
		if (pl->current->argv[iter + 1])
		{
			if (safe_write(1, " ", 1) == -1)
				child_exit_handle(sh, pl, 1);
		}
		iter++;
	}
	if (nflag == 0)
	{
		if (safe_write(1, "\n", 1) == -1)
			child_exit_handle(sh, pl, 1);
	}
	child_exit_handle(sh, pl, 0);
}
