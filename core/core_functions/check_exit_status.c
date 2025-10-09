/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_exit_status.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 12:28:15 by jromann           #+#    #+#             */
/*   Updated: 2025/10/09 16:40:05 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_exit_status(char *buf, t_sh *sh)
{
	if (sh->exit_code == -1 || sh->exit || buf == NULL)
	{
		if (sh->envp.vars)
			free2d(&sh->envp.vars);
		if (sh->exit || buf == NULL)
		{
			if (buf)
				free(buf);
			if (safe_write(1, "exit\n", 5) == -1)
				exit(1);
			if (sh->exit_code == -1)
				exit(1);
			exit(sh->exit_code);
		}
		free(buf);
		if (sh->exit_code == -1)
			exit(1);
		exit(sh->exit_code);
	}
}
