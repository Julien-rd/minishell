/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_cmd_error.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 09:09:33 by jromann           #+#    #+#             */
/*   Updated: 2025/09/27 11:56:46 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_error(t_exec *data, t_cmd *cmd)
{
	long long	exit_code;
	size_t		iter;

	iter = 0;
	if (!cmd->cmd[1])
		child_exit_handle(data, cmd, data->exit_code);
	if (cmd->cmd[1][0] == '0' && cmd->cmd[1][1] == '\0')
		child_exit_handle(data, cmd, 0);
	exit_code = ft_atoll(cmd->cmd[1]);
	if (exit_code == 0)
	{
		if (write(2, "exit: ", 6) == -1)
			perror("write");
		if (write(2, cmd->cmd[1], ft_strlen(cmd->cmd[1])) == -1)
			perror("write");
		if (write(2, ": numeric argument required\n", 28) == -1)
			perror("write");
		child_exit_handle(data, cmd, 2);
	}
	if (cmd->cmd[2])
	{
		if (write(2, "exit: too many arguments\n", 25) == -1)
			perror("write");
		child_exit_handle(data, cmd, 1);
	}
	child_exit_handle(data, cmd, exit_code);
}

void	export_unset_error(t_exec *data, t_cmd *cmd, int cmd_flag)
{
	if (cmd_flag == EXPORT && data->internal_errcode == -1)
		write(2, "export: malloc failed\n", 22);
	else if (cmd_flag == UNSET && data->internal_errcode == 1) // SOLLTE ES HIER
		//-1 sein?
		write(2, "unset: malloc failed\n", 21);
	child_exit_handle(data, cmd, 1);
}

void	cd_error(t_exec *data, t_cmd *cmd)
{
	if (data->internal_errcode == -2)
	{
		write(2, "cd: too many arguments\n", 23);
		child_exit_handle(data, cmd, 1);
	}
	else if (!data->pipe_count)
	{
		if (data->internal_errcode != 0)
		{
			write(2, "cd: ", 4);
			errno = data->exit_code;
			perror(cmd->cmd[1]);
			child_exit_handle(data, cmd, 1);
		}
	}
	else if (chdir(cmd->cmd[1]) == -1)
	{
		write(2, "cd: ", 4);
		perror(cmd->cmd[1]);
		child_exit_handle(data, cmd, 1);
	}
	child_exit_handle(data, cmd, 0);
}

void	internal_cmd_error(t_exec *data, t_cmd *cmd, int flag)
{
	long long	exit_code;
	size_t		iter;

	iter = 0;
	if (flag == -1 && data->cmd_flag != EXIT)
		invalid_option(data, cmd);
	if (data->cmd_flag == CD)
		cd_error(data, cmd);
	if (data->cmd_flag == EXPORT || data->cmd_flag == UNSET)
		export_unset_error(data, cmd, data->cmd_flag);
	if (data->cmd_flag == EXIT)
		exit_error(data, cmd);
	child_exit_handle(data, cmd, 0);
}
