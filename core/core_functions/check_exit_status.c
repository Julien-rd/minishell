/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_exit_status.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 12:28:15 by jromann           #+#    #+#             */
/*   Updated: 2025/10/10 10:06:57 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_exit_status(t_sh *sh)
{
	if (sh->exit_code == -1 || sh->exit || sh->buf == NULL)
	{
		if (sh->envp.vars)
			free2d(&sh->envp.vars);
		if (sh->exit || sh->buf == NULL)
		{
			if (sh->buf)
				free(sh->buf);
			if (safe_write(1, "exit\n", 5) == -1)
				exit(1);
			if (sh->exit_code == -1)
				exit(1);
			exit(sh->exit_code);
		}
		free(sh->buf);
		if (sh->exit_code == -1)
			exit(1);
		exit(sh->exit_code);
	}
}
