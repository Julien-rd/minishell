/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 11:58:58 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/29 10:31:25 by jromann          ###   ########.fr       */
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

int	unset(char **cmd, t_exec *data)
{
	char	*entry;
	int		envp_pos;
	size_t	iter;

	iter = 1;
	while (cmd[iter])
	{
		if (var_check(cmd[iter]) == -1)
		{
			iter++;
			continue ;
		}
		envp_pos = ft_find_paths(data->envp, cmd[iter]);
		if (envp_pos != -1)
		{
			entry = ft_calloc(1, 1);
			if (!entry)
				return (perror("unset"), -1);
			free(data->envp[envp_pos]);
			data->envp[envp_pos] = entry;
		}
		iter++;
	}
	return (0);
}
