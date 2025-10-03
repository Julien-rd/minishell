/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 19:01:15 by jromann           #+#    #+#             */
/*   Updated: 2025/10/03 14:28:33 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exit_cmd(t_pipeline *pl, t_sh *sh)
{
	long long	exit_code;

	exit_code = -1;
	if (!pl->count)
	{
		if (!pl->current->argv[1])
		{
			sh->exit = 1;
			return (0);
		}
		exit_code = ft_atoll(pl->current->argv[1]);
		if (!(pl->current->argv[1][0] == '0' && pl->current->argv[1][1] == '\0')
			&& exit_code == 0)
			sh->exit = 1;
	}
	return (0);
}
