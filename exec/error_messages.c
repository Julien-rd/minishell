/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_messages.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:58:02 by jromann           #+#    #+#             */
/*   Updated: 2025/10/02 11:31:47 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	invalid_option(t_sh *sh, t_cmd *cmd)
{
	write(2, cmd->cmd[0], ft_strlen(cmd->cmd[0]));
	write(2, " ", 1);
	write(2, &cmd->cmd[1][0], 1);
	if (cmd->cmd[1][1])
		write(2, &cmd->cmd[1][1], 1);
	write(2, ": invalid option\n", 17);
	child_exit_handle(sh, cmd, 2);
}

static void	prepare_arg(char *argv[3], t_sh *sh, t_cmd *cmd)
{
	char	minishell[10];

	ft_strlcpy(minishell, "minishell", 10);
	argv[0] = minishell;
	argv[1] = ft_strdup(cmd->cmd[0]);
	if (!argv[1])
	{
		perror("malloc");
		child_exit_handle(sh, cmd, 1);
	}
	argv[2] = NULL;
}

static void	execute_if_cmd_not_found(char *path, t_sh *sh, t_cmd *cmd, int *error)
{
	int		fd;
	char	*argv[3];

	fd = open(cmd->cmd[0], O_RDONLY);
	if (fd != -1)
	{
		if(dup2(fd, STDIN_FILENO) == -1)
			return (perror("dup2"), close(fd), child_exit_handle(sh, cmd, 1));
		close(fd);
		prepare_arg(argv, sh, cmd);
		execve("./minishell", argv, sh->envp.vars);
		return (perror("execve"), child_exit_handle(sh, cmd, 1));
	}
	*error = errno;
}

void	execve_fail(char *path, int error, t_sh *sh, t_cmd *cmd)
{
	struct stat	st;
	int			fd;
	int			child_exit_code;
	char		*argv[3];

	if (errno == ENOEXEC)
		execute_if_cmd_not_found(path, sh, cmd, &error);
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
			error = EISDIR;
	errno = error;
	perror(path);
	if (errno == EACCES || errno == ENOEXEC || errno == EISDIR)
		child_exit_handle(sh, cmd, 126);
	if (errno == ENOENT)
		child_exit_handle(sh, cmd, 127);
	child_exit_handle(sh, cmd, 1);
}

void	command_fail(char *path, t_sh *sh, t_cmd *cmd)
{
	if (errno == EACCES)
	{
		if (safe_write(2, cmd->cmd[0], ft_strlen(cmd->cmd[0])) == -1)
			child_exit_handle(sh, cmd, 1);
		if (write(2, ": Permission denied\n", 20) == -1)
			child_exit_handle(sh, cmd, 1);
		child_exit_handle(sh, cmd, 126);
	}
	else if (errno == ENOENT || cmd->cmd[0][0] == 0)
	{
		if(cmd->cmd[0][0] == 0)
		{
			if (safe_write(2, "''", 2) == -1)
				child_exit_handle(sh, cmd, 1);
		}
		else if (safe_write(2, cmd->cmd[0], ft_strlen(cmd->cmd[0])) == -1)
			child_exit_handle(sh, cmd, 1);
		if (safe_write(2, ": command not found\n", 20) == -1)
			child_exit_handle(sh, cmd, 1);
		child_exit_handle(sh, cmd, 127);
	}
	perror(cmd->cmd[0]);
	child_exit_handle(sh, cmd, 1);
}

void	builtin_handler(t_sh *sh, t_cmd *cmd)
{
	int	flag;

	flag = options_check(cmd);
	if (cmd->cmd_flag == ECHO)
		echo(sh, cmd, flag);
	else if (cmd->cmd_flag == PWD)
		pwd(sh, cmd, flag);
	else if (cmd->cmd_flag == ENV)
		env(sh->envp.vars, sh, cmd, flag);
	else
		internal_cmd_error(sh, cmd, flag);
}
