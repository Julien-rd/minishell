/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 11:58:58 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/06 18:00:54 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	var_check(char *param)
{
	size_t	iter;

	if (!ft_isalpha(param[0]) && param[0] != '_')
		return (-1);
	iter = 1;
	while (param[iter])
	{
		if (!ft_isalnum(param[iter]) && param[iter] != '_')
			return (-1);
		iter++;
	}
	return (0);
}

int	unset(char **cmd, t_pipeline *pl, t_sh *sh)
{
	char	*entry;
	int		envp_pos;
	size_t	iter;

	iter = 1;
	if (pl->count)
		return (0);
	while (cmd[iter])
	{
		if (var_check(cmd[iter]) != -1)
		{
			envp_pos = ft_find_paths(sh->envp.vars, cmd[iter]);
			if (envp_pos != -1)
			{
				entry = ft_calloc(1, 1);
				if (!entry)
					return (perror("unset"), -1);
				free(sh->envp.vars[envp_pos]);
				sh->envp.vars[envp_pos] = entry;
			}
		}
		iter++;
	}
	return (0);
}
