/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_messages.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:58:02 by jromann           #+#    #+#             */
/*   Updated: 2025/10/06 15:37:27 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long long	ft_atoll(const char *str)
{
	long long			i;
	int					sign;
	unsigned long long	tmp_num;

	i = 0;
	sign = 1;
	tmp_num = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			sign = -1;
	if (str[i] < '0' || str[i] > '9')
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (tmp_num > (ULLONG_MAX - str[i] - '0') / 10)
			return (0);
		tmp_num = tmp_num * 10 + str[i++] - '0';
	}
	if ((sign == 1 && (tmp_num > LLONG_MAX)) || str[i])
		return (0);
	else if (tmp_num > (unsigned long long)LLONG_MAX + 1)
		return (0);
	return (sign * (long long)tmp_num);
}

void	invalid_option(t_pipeline *pl, t_sh *sh)
{
	write(2, pl->current->argv[0], ft_strlen(pl->current->argv[0]));
	write(2, ": ", 2);
	write(2, &pl->current->argv[1][0], 1);
	if (pl->current->argv[1][1])
		write(2, &pl->current->argv[1][1], 1);
	write(2, ": invalid option\n", 17);
	child_exit_handle(sh, pl, NULL, 2);
}

static void	execute_if_cmd_not_found(char *path, t_pipeline *pl, t_sh *sh,
		int *error)
{
	int		fd;
	char	*argv[3];
	char	minishell[10];

	fd = open(pl->current->argv[0], O_RDONLY);
	if (fd != -1)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
			return (perror("dup2"), close(fd), child_exit_handle(sh, pl, path,
					1));
		close(fd);
		ft_strlcpy(minishell, "minishell", 10);
		argv[0] = minishell;
		argv[1] = ft_strdup(pl->current->argv[0]);
		if (!argv[1])
		{
			perror("malloc");
			child_exit_handle(sh, pl, NULL, 1);
		}
		argv[2] = NULL;
		execve("./minishell", argv, sh->envp.vars);
		return (perror("execve"), child_exit_handle(sh, pl, path, 1));
	}
	*error = errno;
}

void	execve_fail(char *path, int error, t_pipeline *pl, t_sh *sh)
{
	struct stat	st;

	if (errno == ENOEXEC)
		execute_if_cmd_not_found(path, pl, sh, &error);
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		error = EISDIR;
	errno = error;
	perror(path);
	if (errno == EACCES || errno == ENOEXEC || errno == EISDIR)
		child_exit_handle(sh, pl, path, 126);
	if (errno == ENOENT)
		child_exit_handle(sh, pl, path, 127);
	child_exit_handle(sh, pl, path, 1);
}

void	command_fail(char *path, t_pipeline *pl, t_sh *sh)
{
	if (errno == EACCES)
	{
		if (safe_write(2, pl->current->argv[0],
				ft_strlen(pl->current->argv[0])) == -1)
			child_exit_handle(sh, pl, path, 1);
		if (safe_write(2, ": Permission denied\n", 20) == -1)
			child_exit_handle(sh, pl, path, 1);
		child_exit_handle(sh, pl, path, 126);
	}
	else if (errno == ENOENT || pl->current->argv[0][0] == 0)
	{
		if (pl->current->argv[0][0] == 0)
		{
			if (safe_write(2, "''", 2) == -1)
				child_exit_handle(sh, pl, path, 1);
		}
		else if (safe_write(2, pl->current->argv[0],
				ft_strlen(pl->current->argv[0])) == -1)
			child_exit_handle(sh, pl, path, 1);
		if (safe_write(2, ": command not found\n", 20) == -1)
			child_exit_handle(sh, pl, path, 1);
		child_exit_handle(sh, pl, path, 127);
	}
	perror(pl->current->argv[0]);
	child_exit_handle(sh, pl, path, 1);
}
