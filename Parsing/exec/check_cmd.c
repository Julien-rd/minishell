/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 08:36:18 by jromann           #+#    #+#             */
/*   Updated: 2025/09/22 10:02:15 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	flag_check(t_cmd *cmd)
{
	size_t	iter;

	iter = 1;
	if (!cmd)
		return (-1);
	while (cmd->cmd[iter])
	{
		if (cmd->cmd[iter][0] == '-')
		{
			if (!ft_strncmp(cmd->cmd[0], "cd", 3) && !ft_strncmp(cmd->cmd[1],
					"-n", 3))
				return (2);
			return (0);
		}
		iter++;
	}
	return (1);
}

int	check_cmd(t_exec *data, t_cmd *cmd)
{
	int	flag;

	flag = flag_check(cmd);
	if (flag)
	{
		if (!ft_strncmp(cmd->cmd[0], "echo", 5))
			echo(cmd->cmd, flag);
		if (!ft_strncmp(cmd->cmd[0], "cd", 3))
			cd(cmd->cmd);
		if (!ft_strncmp(cmd->cmd[0], "pwd", 4))
			pwd(cmd->cmd);
		if (!ft_strncmp(cmd->cmd[0], "export", 7))
			export(cmd->cmd, data);
		if (!ft_strncmp(cmd->cmd[0], "unset", 6))
			unset(cmd->cmd);
		if (!ft_strncmp(cmd->cmd[0], "env", 4))
			env(cmd->cmd);
		if (!ft_strncmp(cmd->cmd[0], "exit", 5))
			exit_cmd(data, cmd);
	}
}
