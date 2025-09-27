/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 08:36:18 by jromann           #+#    #+#             */
/*   Updated: 2025/09/27 11:56:46 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	options_check(t_cmd *cmd)
{
	size_t	iter;
	size_t	n_iter;

	iter = 1;
	if (!cmd)
		return (-1);
	if (!ft_strncmp(cmd->cmd[0], "echo", 5) && cmd->cmd[1])
	{
		while (cmd->cmd[iter] && cmd->cmd[iter][0] == '-')
		{
			n_iter = 1;
			while (cmd->cmd[iter][n_iter] == 'n')
				n_iter++;
			if (cmd->cmd[iter][n_iter])
				break ;
			iter++;
		}
		if (ft_strncmp(cmd->cmd[iter], "-e", 3) && ft_strncmp(cmd->cmd[iter],
				"-E", 3))
			return (-1);
		return (iter - 1);
	}
	if (cmd->cmd[1] != NULL && cmd->cmd[1][0] == '-')
		return (-1);
	return (0);
}

int	cmd_flag(t_exec *data, t_cmd *cmd)
{
	data->cmd_flag = EXTERNAL;
	data->internal_errcode = 0;
	if (cmd->cmd[0] == NULL)
		return (0);
	if (!ft_strncmp(cmd->cmd[0], "echo", 5))
		return (data->cmd_flag = ECHO, 0);
	if (!ft_strncmp(cmd->cmd[0], "cd", 3))
		return (data->cmd_flag = CD, 1);
	if (!ft_strncmp(cmd->cmd[0], "pwd", 4))
		return (data->cmd_flag = PWD, 0);
	if (!ft_strncmp(cmd->cmd[0], "export", 7))
		return (data->cmd_flag = EXPORT, 1);
	if (!ft_strncmp(cmd->cmd[0], "unset", 6))
		return (data->cmd_flag = UNSET, 1);
	if (!ft_strncmp(cmd->cmd[0], "env", 4))
		return (data->cmd_flag = ENV, 0);
	if (!ft_strncmp(cmd->cmd[0], "exit", 5))
		return (data->cmd_flag = EXIT, 1);
	return (0);
}
