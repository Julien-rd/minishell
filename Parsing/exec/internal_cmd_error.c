/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_cmd_error.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 09:09:33 by jromann           #+#    #+#             */
/*   Updated: 2025/09/23 17:53:04 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	internal_cmd_error(t_exec *data, t_cmd *cmd)
{
	long long exit_code;
	size_t iter;

	iter = 0;
	if (data->cmd_flag == CD)
	{
		if (data->internal_errcode == -2)
		{
			write(2, "cd: too many arguments\n", 23);
			child_exit_handle(data, cmd, 1);
		}
		else if (!data->pipe_count)
		{
			if (data->internal_errcode == -1)
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
	if (data->cmd_flag == EXPORT)
	{
		if (data->internal_errcode == -1)
			write(2, "export: malloc failed\n", 22);
		child_exit_handle(data, cmd, 1);
	}
	if (data->cmd_flag == UNSET)
	{
		if (data->internal_errcode == 1)
			write(2, "unset: malloc failed\n", 21);
		child_exit_handle(data, cmd, 1);
	}
	if (data->cmd_flag == EXIT)
	{
		if (cmd->cmd[1])
		{
			exit_code = exit_status(cmd->cmd[1]);
			while (cmd->cmd[1][iter])
			{
				if (!ft_isdigit(cmd->cmd[1][iter]))
					exit_code = -1;
				iter++;
			}
			if (exit_code == -1)
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
		if (data->internal_errcode == 12)
			child_exit_handle(data, cmd, 12);
		if (data->internal_errcode == 2)
		{
			write(2, "exit --invalid option\n", 22);
			child_exit_handle(data, cmd, 2);
		}
	}
	child_exit_handle(data, cmd, 0);
}