/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_exit_status.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 12:28:15 by jromann           #+#    #+#             */
/*   Updated: 2025/10/09 12:41:28 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_exit_status(char *buf, int exit_code, t_sh *sh)
{
	if ((exit_code == -1 && g_current_signal == 0) || sh->exit || buf == NULL)
	{
		if (sh->envp.vars)
			free2d(&sh->envp.vars);
		if (sh->exit || buf == NULL)
		{
			if (buf)
				free(buf);
			if (safe_write(1, "exit\n", 5) == -1)
				exit(1);
			exit(exit_code);
		}
		free(buf);
		exit(1);
	}
}