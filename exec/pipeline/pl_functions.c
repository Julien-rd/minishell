/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_functions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:28:40 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/03 14:59:40 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_cmds(t_pipeline *pl, t_sh *sh)
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
	if(!pl->cmds)
		return(free(pl->position), -1);
	while (cmd_iter <= pl->count)
	{
		find_start(pl, sh, cmd_iter);
		if (cmd_init(&pl->cmds[cmd_iter]) == -1)
			return (perror("cmd_init"), -1);
		cmd_flag(sh, &pl->cmds[cmd_iter]);
		cmd_iter++;
	}
	return (0);
}

void	own_cmd_exec(t_pipeline *pl, t_sh *sh)
{
	int	flag;

	if (pl->current->argv[0] == NULL)
	{
		sh->internal_errcode = 0;
		return ;
	}
	flag = options_check(pl->current);
	if (pl->current->cmd_flag == CD && !flag)
		sh->internal_errcode = cd(sh, pl->current->argv, pl->count);
	if (pl->current->cmd_flag == EXIT)
		sh->internal_errcode = exit_cmd(pl, sh);
	if (pl->current->cmd_flag == EXPORT && !flag)
		sh->internal_errcode = export(pl->current->argv, sh);
	if (pl->current->cmd_flag == UNSET && !flag)
		sh->internal_errcode = unset(pl->current->argv, sh);
	else
		sh->internal_errcode = 0;
}

void	child_process(t_pipeline *pl, t_sh *sh)
{
	char	*path;
	int		flag;

	setup_child_signals();
	if (setup_redirect(sh, pl) == -1)
		child_exit_handle(sh, pl, 1);
	if (pl->current->argv[0] == NULL)
		child_exit_handle(sh, pl, 0);
	if (pl->current->cmd_flag != EXTERNAL)
		builtin_handler(pl, sh);
	path = ft_getpath(sh->envp.vars, pl->current->argv[0]);
	if (path == NULL || pl->current->argv[0][0] == 0)
		command_fail(path, pl, sh);
	execve(path, pl->current->argv, sh->envp.vars);
	execve_fail(path, errno, pl, sh);
}

int	parent_process(t_pipeline *pl, t_sh *sh)
{
	if (pl->iter != 0)
	{
		if (close(pl->prev_fd) == -1)
			return (perror("close"), -1);
	}
	if (pl->iter != pl->count)
	{
		pl->prev_fd = pl->fd[0];
		if (close(pl->fd[1]) == -1)
			return (perror("close"), -1);
	}
	return (0);
}

int	kill_children(t_pipeline *pl, t_sh *sh)
{
	int		status;
	int		return_value;
	pid_t	pid;

	return_value = -1;
	while (1)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
		{
			if (errno == ECHILD)
				break ;
			else if (errno == EINTR)
				continue ;
			else
				break ;
		}
		if (pid == pl->last_pid)
		{
			if (WIFEXITED(status))
				return_value = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				return_value = 128 + WTERMSIG(status);
			else if (WIFSTOPPED(status))
				return_value = 128 + WSTOPSIG(status);
		}
	}
	if (errno != ECHILD)
		return (perror("waitpid"), -1);
	return (return_value);
}
