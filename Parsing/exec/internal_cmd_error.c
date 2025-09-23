/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_cmd_error.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 09:09:33 by jromann           #+#    #+#             */
/*   Updated: 2025/09/23 12:16:23 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	internal_cmd_error(t_exec *data, t_cmd *cmd)
{
	if (data->cmd_flag == CD)
	{
		if (data->internal_errcode == -1)
			write(2, "getcwd failed\n", 14); // mit errno besser machen
		if (data->internal_errcode == -2)
			write(2, "cd: too many arguments\n", 23);
		child_exit_handle(data, cmd, 1);
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
		if (data->internal_errcode == 12)
			child_exit_handle(data, cmd, 12);
		if (data->internal_errcode == 2)
		{
			write(2, "exit --invalid option\n", 22);
			child_exit_handle(data, cmd, 1);
		}
	}
	child_exit_handle(data, cmd, 0);
}