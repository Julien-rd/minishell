/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_messages.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:58:02 by jromann           #+#    #+#             */
/*   Updated: 2025/10/03 10:29:55 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	invalid_option(t_pipeline *pl, t_sh *sh)
{
	write(2, pl->current->argv[0], ft_strlen(pl->current->argv[0]));
	write(2, " ", 1);
	write(2, &pl->current->argv[1][0], 1);
	if (pl->current->argv[1][1])
		write(2, &pl->current->argv[1][1], 1);
	write(2, ": invalid option\n", 17);
	child_exit_handle(sh, pl, 2);
}

static void	prepare_arg(char *argv[3], t_pipeline *pl, t_sh *sh)
{
	char	minishell[10];

	ft_strlcpy(minishell, "minishell", 10);
	argv[0] = minishell;
	argv[1] = ft_strdup(pl->current->argv[0]);
	if (!argv[1])
	{
		perror("malloc");
		child_exit_handle(sh, pl, 1);
	}
	argv[2] = NULL;
}

static void	execute_if_cmd_not_found(char *path, t_pipeline *pl, t_sh *sh,
		int *error)
{
	int		fd;
	char	*argv[3];

	fd = open(pl->current->argv[0], O_RDONLY);
	if (fd != -1)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
			return (perror("dup2"), close(fd), child_exit_handle(sh, pl, 1));
		close(fd);
		prepare_arg(argv, pl, sh);
		execve("./minishell", argv, sh->envp.vars);
		return (perror("execve"), child_exit_handle(sh, pl, 1));
	}
	*error = errno;
}

void	execve_fail(char *path, int error, t_pipeline *pl, t_sh *sh)
{
	struct stat	st;
	int			fd;
	int			child_exit_code;
	char		*argv[3];

	if (errno == ENOEXEC)
		execute_if_cmd_not_found(path, pl, sh, &error);
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		error = EISDIR;
	errno = error;
	perror(path);
	if (errno == EACCES || errno == ENOEXEC || errno == EISDIR)
		child_exit_handle(sh, pl, 126);
	if (errno == ENOENT)
		child_exit_handle(sh, pl, 127);
	child_exit_handle(sh, pl, 1);
}

void	command_fail(char *path, t_pipeline *pl, t_sh *sh)
{
	if (errno == EACCES)
	{
		if (safe_write(2, pl->current->argv[0],
				ft_strlen(pl->current->argv[0])) == -1)
			child_exit_handle(sh, pl, 1);
		if (write(2, ": Permission denied\n", 20) == -1)
			child_exit_handle(sh, pl, 1);
		child_exit_handle(sh, pl, 126);
	}
	else if (errno == ENOENT || pl->current->argv[0][0] == 0)
	{
		if (pl->current->argv[0][0] == 0)
		{
			if (safe_write(2, "''", 2) == -1)
				child_exit_handle(sh, pl, 1);
		}
		else if (safe_write(2, pl->current->argv[0],
				ft_strlen(pl->current->argv[0])) == -1)
			child_exit_handle(sh, pl, 1);
		if (safe_write(2, ": command not found\n", 20) == -1)
			child_exit_handle(sh, pl, 1);
		child_exit_handle(sh, pl, 127);
	}
	perror(pl->current->argv[0]);
	child_exit_handle(sh, pl, 1);
}

void	builtin_handler(t_pipeline *pl, t_sh *sh)
{
	int	flag;

	flag = options_check(pl->current);
	if (pl->current->cmd_flag == ECHO)
		echo(pl, sh, flag);
	else if (pl->current->cmd_flag == PWD)
		pwd(sh, pl, flag);
	else if (pl->current->cmd_flag == ENV)
		env(pl, sh, flag);
	else
		internal_cmd_error(pl, sh, flag);
}
