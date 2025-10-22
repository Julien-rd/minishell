/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:28:40 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/22 14:09:48 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pl_setup(t_pipeline *pl, t_sh *sh)
{
	size_t	cmd_iter;

	cmd_iter = 0;
	pl->iter = 0;
	pl->hdoc_iter = 0;
	pl->current = NULL;
	pl->count = count_pipes(sh);
	if (init_pipe_pos(pl, sh) == -1)
		return (-1);
	pl->cmds = malloc((pl->count + 1) * sizeof(t_cmd));
	if (!pl->cmds)
		return (perror("setup_cmds"), free(pl->position), -1);
	while (cmd_iter <= pl->count)
	{
		find_start(pl, sh, cmd_iter);
		if (cmd_init(&pl->cmds[cmd_iter]) == -1)
			return (free(pl->position), free_cmds(pl, cmd_iter), -1);
		cmd_flag(sh, &pl->cmds[cmd_iter]);
		cmd_iter++;
	}
	return (0);
}

int	own_cmd_exec(t_pipeline *pl, t_sh *sh)
{
	int	flag;

	if (pl->current->argv[0] == NULL)
	{
		sh->internal_errcode = 0;
		return (sh->internal_errcode);
	}
	flag = options_check(pl->current);
	if (pl->current->cmd_flag == CD && !flag)
		sh->internal_errcode = cd(sh, pl->current->argv, pl->count);
	else if (pl->current->cmd_flag == EXIT)
		sh->internal_errcode = exit_cmd(pl, sh);
	else if (pl->current->cmd_flag == EXPORT && !flag)
		sh->internal_errcode = export(pl->current->argv, pl, sh);
	else if (pl->current->cmd_flag == UNSET && !flag)
		sh->internal_errcode = unset(pl->current->argv, pl, sh);
	else
		sh->internal_errcode = 0;
	return (sh->internal_errcode);
}

void	child_process(t_pipeline *pl, t_sh *sh)
{
	char	*path;

	setup_child_signals();
	if (setup_redirect(sh, pl) == -1)
		child_exit(sh, pl, 1);
	if (pl->current->argv[0] == NULL)
		child_exit(sh, pl, 0);
	if (pl->current->cmd_flag != EXTERNAL)
		builtin_handler(pl, sh);
	path = ft_getpath(sh->envp.vars, pl->current->argv[0]);
	if (path == NULL || pl->current->argv[0][0] == 0)
	{
		if (path && !ft_strchr(pl->current->argv[0], '/'))
			free(path);
		command_fail(pl, sh);
	}
	execve(path, pl->current->argv, sh->envp.vars);
	execve_fail(path, errno, pl, sh);
}

int	parent_process(t_pipeline *pl)
{
	if (pl->iter != 0)
	{
		if (close(pl->prev_fd) == -1)
		{
			perror("close");
			if (pl->iter != pl->count)
			{
				close(pl->fd[0]);
				close(pl->fd[1]);
			}
			return (-1);
		}
	}
	if (pl->iter != pl->count)
	{
		pl->prev_fd = pl->fd[0];
		if (close(pl->fd[1]) == -1)
			return (perror("close"), close(pl->fd[0]), -1);
	}
	return (0);
}

int	kill_children(t_pipeline *pl, t_sh *sh)
{
	int		status;
	pid_t	pid;

	sh->exit_code = -1;
	while (1)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
		{
			if (errno == EINTR)
				continue ;
			else
				break ;
		}
		if (pid == pl->last_pid)
		{
			if (WIFEXITED(status))
				sh->exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status) || WIFSTOPPED(status))
				sh->exit_code = 128 + WTERMSIG(status);
		}
	}
	if (errno != ECHILD)
		return (perror("waitpid"), -1);
	return (sh->exit_code);
}
