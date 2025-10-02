/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_cmd_error.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 09:09:33 by jromann           #+#    #+#             */
/*   Updated: 2025/10/02 11:36:13 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_error(t_sh *sh, t_cmd *cmd)
{
	long long	exit_code;
	size_t		iter;

	iter = 0;
	if (!cmd->cmd[1])
		child_exit_handle(sh, cmd, sh->exit_code);
	if (cmd->cmd[1][0] == '0' && cmd->cmd[1][1] == '\0')
		child_exit_handle(sh, cmd, 0);
	exit_code = ft_atoll(cmd->cmd[1]);
	if (exit_code == 0)
	{
		if (safe_write(2, "exit: ", 6) == -1)
			child_exit_handle(sh, cmd, 1);
		if (safe_write(2, cmd->cmd[1], ft_strlen(cmd->cmd[1])) == -1)
			child_exit_handle(sh, cmd, 1);
		if (safe_write(2, ": numeric argument required\n", 28) == -1)
			child_exit_handle(sh, cmd, 1);
		child_exit_handle(sh, cmd, 2);
	}
	if (cmd->cmd[2])
	{
		safe_write(2, "exit: too many arguments\n", 25);
		child_exit_handle(sh, cmd, 1);
	}
	child_exit_handle(sh, cmd, exit_code);
}

void	export_unset_error(t_sh *sh, t_cmd *cmd, int cmd_flag)
{
	if (cmd_flag == EXPORT && sh->internal_errcode == -1)
	{
		write(2, "export: malloc failed\n", 22);
		child_exit_handle(sh, cmd, 1);
	}
	if (cmd_flag == EXPORT && sh->internal_errcode == 1)
		child_exit_handle(sh, cmd, 1);
	else if (cmd_flag == UNSET && sh->internal_errcode == 1)
		write(2, "unset: malloc failed\n", 21);
	child_exit_handle(sh, cmd, 0);
}

void	cd_error(t_sh *sh, t_cmd *cmd)
{
	if (sh->internal_errcode == -2)
	{
		write(2, "cd: too many arguments\n", 23);
		child_exit_handle(sh, cmd, 1);
	}
	else if (!sh->pipe.count)
	{
		if (sh->internal_errcode != 0)
		{
			write(2, "cd: ", 4);
			errno = sh->exit_code;
			perror(cmd->cmd[1]);
			child_exit_handle(sh, cmd, 1);
		}
	}
	else if (chdir(cmd->cmd[1]) == -1)
	{
		write(2, "cd: ", 4);
		perror(cmd->cmd[1]);
		child_exit_handle(sh, cmd, 1);
	}
	child_exit_handle(sh, cmd, 0);
}

void	internal_cmd_error(t_sh *sh, t_cmd *cmd, int flag)
{
	long long	exit_code;
	size_t		iter;

	iter = 0;
	if (flag == -1 && cmd->cmd_flag != EXIT)
		invalid_option(sh, cmd);
	if (cmd->cmd_flag == CD)
		cd_error(sh, cmd);
	if (cmd->cmd_flag == EXPORT || cmd->cmd_flag == UNSET)
		export_unset_error(sh, cmd, cmd->cmd_flag);
	if (cmd->cmd_flag == EXIT)
		exit_error(sh, cmd);
	child_exit_handle(sh, cmd, 0);
}
