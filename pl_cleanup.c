/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_cleanup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:58:13 by jromann           #+#    #+#             */
/*   Updated: 2025/10/09 15:25:56 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pl_cleanup(t_pipeline *pl, t_sh *sh, int flag)
{
	free_cmds(pl, pl->count + 1);
	free(pl->position);
	if (flag == FAILURE)
		return (-1);
	return (kill_children(pl, sh));
}