/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_messages.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:58:02 by jromann           #+#    #+#             */
/*   Updated: 2025/09/26 10:53:57 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	invalid_option(t_exec *data, t_cmd *cmd)
{
	write(2, cmd->cmd[0], ft_strlen(cmd->cmd[0]));
	write(2, " ", 1);
	write(2, &cmd->cmd[1][0], 1);
	if (cmd->cmd[1][1])
		write(2, &cmd->cmd[1][1], 1);
	write(2, ": invalid option\n", 17);
	child_exit_handle(data, cmd, 2);
}

void	execve_fail(char *path, int error, t_exec *data, t_cmd *cmd)
{
	struct stat	st;

	if (stat(path, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			errno = EISDIR;
			perror(path);
			child_exit_handle(data, cmd, 126);
		}
		else if (S_ISREG(st.st_mode))
		{
			errno = error;
			perror(path);
			if (errno == EACCES || errno == ENOEXEC)
				child_exit_handle(data, cmd, 126);
		}
	}
	// if (error == ENOMEM || error == ETXTBSY || error == EFAULT
	// 	|| error == EINVAL)
	perror(path);
	if (errno == ENOENT)
		child_exit_handle(data, cmd, 127);
	child_exit_handle(data, cmd, 1);
}
static int	safe_write(int fd, char *buf, size_t len)
{
	if (write(fd, buf, len) == -1)
		return (perror("write"), -1);
	return (0);
}
void	command_fail(char *path, t_exec *data, t_cmd *cmd)
{
	if (errno == EACCES)
	{
		if (safe_write(2, cmd->cmd[0], ft_strlen(cmd->cmd[0])) == -1)
			child_exit_handle(data, cmd, 1);
		if (write(2, ": Permission denied\n", 20) == -1)
			child_exit_handle(data, cmd, 1);
		child_exit_handle(data, cmd, 126);
	}
	else if (errno == ENOENT)
	{
		if (safe_write(2, cmd->cmd[0], ft_strlen(cmd->cmd[0])) == -1)
			child_exit_handle(data, cmd, 1);
		if (safe_write(2, ": command not found\n", 20) == -1)
			child_exit_handle(data, cmd, 1);
		child_exit_handle(data, cmd, 127);
	}
	else
	{
		perror(cmd->cmd[0]);
		child_exit_handle(data, cmd, 1);
	}
}
void	builtin_handler(t_exec *data, t_cmd *cmd)
{
	int	flag;

	flag = options_check(cmd);
	if (data->cmd_flag == ECHO)
		echo(data, cmd, flag);
	else if (data->cmd_flag == PWD)
		pwd(data, cmd, flag);
	else if (data->cmd_flag == ENV)
		env(data->envp, data, cmd, flag);
	else
		internal_cmd_error(data, cmd, flag);
}
