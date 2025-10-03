/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 19:01:35 by jromann           #+#    #+#             */
/*   Updated: 2025/10/03 10:31:17 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env(t_pipeline *pl, t_sh *sh, int flag)
{
	size_t	iter;

	iter = 0;
	if (!sh->envp.vars)
		child_exit_handle(sh, pl, 1);
	if (flag == -1)
		invalid_option(pl, sh);
	while (sh->envp.vars[iter])
	{
		if (safe_write(1, sh->envp.vars[iter],
				ft_strlen(sh->envp.vars[iter])) == -1)
			child_exit_handle(sh, pl, 1);
		if (sh->envp.vars[iter][0] != '\0' && safe_write(1, "\n", 1) == -1)
			child_exit_handle(sh, pl, 1);
		iter++;
	}
	child_exit_handle(sh, pl, 0);
}
