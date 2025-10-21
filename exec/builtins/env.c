/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 19:01:35 by jromann           #+#    #+#             */
/*   Updated: 2025/10/21 12:26:35 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env(t_pipeline *pl, t_sh *sh, int option_flag)
{
	size_t	iter;

	iter = 0;
	if (option_flag == -1)
		return (invalid_option(pl, sh));
	if (pl->current->argv[1])
	{
		safe_write(2, "env: too many arguments\n", 25);
		child_exit(sh, pl, 1);
	}
	while (sh->envp.vars[iter])
	{
		if (safe_write(1, sh->envp.vars[iter],
				ft_strlen(sh->envp.vars[iter])) == -1)
			child_exit(sh, pl, 1);
		if (sh->envp.vars[iter][0] != '\0' && safe_write(1, "\n", 1) == -1)
			child_exit(sh, pl, 1);
		iter++;
	}
	child_exit(sh, pl, 0);
}
