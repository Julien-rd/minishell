/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 08:36:18 by jromann           #+#    #+#             */
/*   Updated: 2025/10/03 17:13:07 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	options_check(t_cmd *cur)
{
	size_t	iter;
	size_t	n_iter;

	iter = 1;
	if (!cur)
		return (-1);
	if (!ft_strncmp(cur->argv[0], "echo", 5) && cur->argv[1])
	{
		while (cur->argv[iter] && cur->argv[iter][0] == '-')
		{
			n_iter = 1;
			while (cur->argv[iter][n_iter] == 'n')
				n_iter++;
			if (cur->argv[iter][n_iter])
				break ;
			iter++;
		}
		if (!(ft_strncmp(cur->argv[iter], "-e", 3) && ft_strncmp(cur->argv[iter],
				"-E", 3)))
			return (-1);
		return (iter - 1);
	}
	if (cur->argv[1] != NULL && cur->argv[1][0] == '-' && cur->argv[1][1])
		return (-1);
	return (0);
}

void	cmd_flag(t_sh *sh, t_cmd*current)
{
	current->cmd_flag = EXTERNAL;
	sh->internal_errcode = 0;
	if (current->argv[0] == NULL)
		return ;
	else if (!ft_strncmp(current->argv[0], "echo", 5))
		current->cmd_flag = ECHO;
	else if (!ft_strncmp(current->argv[0], "cd", 3))
		current->cmd_flag = CD;
	else if (!ft_strncmp(current->argv[0], "pwd", 4))
		current->cmd_flag = PWD;
	else if (!ft_strncmp(current->argv[0], "export", 7))
		current->cmd_flag = EXPORT;
	else if (!ft_strncmp(current->argv[0], "unset", 6))
		current->cmd_flag = UNSET;
	else if (!ft_strncmp(current->argv[0], "env", 4))
		current->cmd_flag = ENV;
	else if (!ft_strncmp(current->argv[0], "exit", 5))
		current->cmd_flag = EXIT;
}
