/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 13:58:47 by jromann           #+#    #+#             */
/*   Updated: 2025/10/04 14:01:05 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_handler(t_pipeline *pl, t_sh *sh)
{
	int	flag;

	flag = options_check(pl->current);
	if (pl->current->cmd_flag == ECHO)
		echo(pl, sh, flag);
	else if (pl->current->cmd_flag == PWD)
		pwd(sh, pl, flag);
	else if (pl->current->cmd_flag == ENV)
		env(pl, sh, flag);
	else
		internal_cmd_error(pl, sh, flag);
}
