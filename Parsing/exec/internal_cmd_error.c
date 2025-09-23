/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_cmd_error.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 09:09:33 by jromann           #+#    #+#             */
/*   Updated: 2025/09/23 12:04:09 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	internal_cmd_error(t_exec *data, t_cmd *cmd)
{
	size_t iter;

	iter = 0;
	if (data->cmd_flag == CD)
	{
		while (cmd->cmd[iter])
			iter++;
		if (iter > 2)
		{
			write(2, "cd: too many arguments\n", 23);
			exit(1);
		}
		if (iter == 1)
		{
			write(2, "cd: needs one argument\n", 23);
			exit(1);
		}
		else if(!data->pipe_count)
		{
			if (data->internal_errcode == -2)
			{
				perror("cd");
			}
		}
		else if (chdir(cmd->cmd[1]) == -1)
		{
			write(2, "cd: ", 4);
			write(2, cmd->cmd[1], ft_strlen(cmd->cmd[1]));
			perror(" ");
		}
		exit(1);
	}
	if (data->cmd_flag == EXPORT)
	{
		if (data->internal_errcode == -1)
			write(2, "export: malloc failed\n", 22);
		exit(1);
	}
	if (data->cmd_flag == UNSET)
	{
		if (data->internal_errcode == 1)
			write(2, "unset: malloc failed\n", 21);
		exit(1);
	}
	if (data->cmd_flag == EXIT)
	{
		if (data->internal_errcode == 12)
			exit(12);
		if (data->internal_errcode == 2)
		{
			write(2, "exit --invalid option\n", 22);
			exit(2);
		}
	}
	exit(0);
}