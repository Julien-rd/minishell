/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 08:36:18 by jromann           #+#    #+#             */
/*   Updated: 2025/09/22 15:46:45 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	flag_check(t_cmd *cmd)
{
	size_t	iter;

	iter = 1;
	if (!cmd)
		return (-1);
	while (cmd->cmd[iter])
	{
		if (cmd->cmd[iter][0] == '-')
		{
			if (!ft_strncmp(cmd->cmd[0], "echo", 5) && !ft_strncmp(cmd->cmd[1],
					"-n", 3))
				return (2);
			return (1);
		}
		iter++;
	}
	return (0);
}

// int	check_cmd(t_exec *data, t_cmd *cmd)
// {
// 	int	flag;

// 	data->cmd_flag = EXTERNAL;
// 	flag = flag_check(cmd);
// 	if (flag)
// 	{
// 		if (!ft_strncmp(cmd->cmd[0], "echo", 5))
// 			return (data->cmd_flag = ECHO, 0);
// 		if (!ft_strncmp(cmd->cmd[0], "cd", 3))
// 			return (data->cmd_flag = INTERNAL, 1);
// 		if (!ft_strncmp(cmd->cmd[0], "pwd", 4))
// 			return (data->cmd_flag = PWD, 0);
// 		if (!ft_strncmp(cmd->cmd[0], "export", 7))
// 			return (data->cmd_flag = INTERNAL, 1);
// 		if (!ft_strncmp(cmd->cmd[0], "unset", 6))
// 			return (data->cmd_flag = INTERNAL, 1);
// 		if (!ft_strncmp(cmd->cmd[0], "env", 4))
// 			return (data->cmd_flag = ENV, 0);
// 		if (!ft_strncmp(cmd->cmd[0], "exit", 5))
// 			return (data->cmd_flag = EXIT, 1);
// 	}
// 	return (0);
// }

int	cmd_flag(t_exec *data, t_cmd *cmd)
{
	data->cmd_flag = EXTERNAL;
	data->internal_errcode = 0;
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
