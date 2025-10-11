/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_exit_status.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 12:28:15 by jromann           #+#    #+#             */
/*   Updated: 2025/10/11 12:48:24 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_exit_status(char *buf, t_sh *sh, int flag)
{
	if (sh->exit_code == -1 || sh->exit || buf == NULL)
	{
		get_next_line(-1);
		if (sh->envp.vars)
			free2d(&sh->envp.vars);
		if (sh->exit || buf == NULL)
		{
			rl_clear_history();
			if (flag == INTERACTIVE && safe_write(1, "exit\n", 5) == -1)
				exit(1);
			if (sh->exit_code == -1)
				exit(1);
			exit(sh->exit_code);
		}
		rl_clear_history();
		if (sh->exit_code == -1)
			exit(1);
		exit(sh->exit_code);
	}
}
