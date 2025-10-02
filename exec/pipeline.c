/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:40:30 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/02 17:00:27 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cmd_init(t_cmd *current)
{
	t_entry	*node;
	size_t	cmd_tokens;
	size_t	cmd_iter;
	size_t	iter;

	node = current->line;
	cmd_tokens = 0;
	while (node && node->spec != PIPE)
	{
		if (node->spec == DEFAULT)
		{
			iter = -1;
			while(node->expanded && node->expanded[++iter])
				cmd_tokens++;
		}
		node = node->next;
	}
	current->argv = malloc((cmd_tokens + 1) * sizeof(char *));
	if (!current->argv)
		return (-1);
	node = current->line;
	cmd_iter = 0;
	while (node && node->spec != PIPE)
	{
		if (node->spec == DEFAULT)
		{
			iter = -1;
			while(node->expanded && node->expanded[++iter])
				current->argv[cmd_iter++] = node->expanded[iter];
		}
		node = node->next;
	}
	current->argv[cmd_iter] = NULL;
	iter = -1;
	return (0);
}

void	child_exit_handle(t_sh *sh, t_pipeline *pl, int errcode)
{
	free2d(&sh->envp.vars);
	free_list(sh->entries);
	// free(sh->pipe.position);
	// free(cmd->cmd);
	if (sh->heredoc)
		free2d(&sh->heredoc);
	exit(errcode);
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

void	parent_line(t_pipeline *pl, t_sh *sh)
{
	if (pl->iter != 0)
	{
		if (close(pl->prev_fd) == -1)
			exit(1);
	}
	if (pl->iter != pl->count)
	{
		pl->prev_fd = pl->fd[0];
		if (close(pl->fd[1]) == -1)
			exit(1);
	}
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

int	own_cmd_exec(t_pipeline *pl, t_sh *sh)
{
	int	flag;

	if (pl->current->argv[0] == NULL)
		return (0);
	flag = options_check(pl->current);
	if (pl->current->cmd_flag == CD && !flag)
		return (cd(sh, pl->current->argv, pl->count));
	if (pl->current->cmd_flag == EXIT)
		return (exit_cmd(sh, pl->count));
	if (pl->current->cmd_flag == EXPORT && !flag)
		return (export(pl->current->argv, sh));
	if (pl->current->cmd_flag == UNSET && !flag)
		return (unset(pl->current->argv, sh));
	return (0);
}

static int	count_pipes(t_sh *sh)
{
	t_entry	*node;
	size_t	pipe_count;

	node = sh->entries;
	pipe_count = 0;
	while (node != NULL)
	{
		if (node->spec == PIPE)
			pipe_count++;
		node = node->next;
	}
	return (pipe_count);
}

static int	init_pipe_pos(t_pipeline *pl, t_sh *sh)
{
	t_entry	*node;
	size_t	counter;
	size_t	iter;

	node = sh->entries;
	counter = 0;
	iter = 0;
	pl->position = malloc(sizeof(int) * (pl->count + 1));
	if (!pl->position)
		return (-1);
	pl->position[iter++] = 0;
	while (node != NULL)
	{
		if (node->spec == PIPE)
			pl->position[iter++] = counter + 1;
		counter++;
		node = node->next;
	}
	return (0);
}

void find_start(t_pipeline *pl, t_sh *sh, size_t line_iter)
{
	t_entry	*node;
	size_t iter;
	
	iter = 0;
	node = sh->entries;
	while(node && iter < pl->position[line_iter])
	{
		node = node->next;
		iter++;
	}
	pl->cmds[line_iter].line = node;
}

int	setup_cmds(t_pipeline *pl, t_sh *sh)
{
	size_t	line_iter;

	line_iter = 0;
	pl->iter = 0;
	pl->hdoc_iter = 0;
	pl->current = NULL;
	pl->count = count_pipes(sh);
	if (init_pipe_pos(pl, sh) == -1)
		return (-1);
	pl->cmds = malloc((pl->count + 1) * sizeof(t_cmd));
	while (line_iter <= pl->count)
	{
		find_start(pl, sh, line_iter);
		if (cmd_init(&pl->cmds[line_iter]) == -1)
			return (perror("cmd_init"), -1);
		cmd_flag(sh, &pl->cmds[line_iter]);
		line_iter++;
	}
	return (0);
}

int	pipe_fork(t_pipeline *pl)
{
	if (pl->iter != pl->count)
			if (pipe(pl->fd) == -1)
				return (perror("pipe"), -1);
	pl->current->pid = fork();
	if (pl->current->pid == -1)
		return (perror("fork"), -1);
	if (pl->iter == pl->count)
		pl->last_pid = pl->current->pid;
	return (0);
}

int	pipeline(t_sh *sh)
{
	t_pipeline	pl;
	
	if (setup_cmds(&pl, sh) == -1)
		return (-1);
	while (pl.iter <= pl.count)
	{
		pl.current = &pl.cmds[pl.iter];
		sh->internal_errcode = own_cmd_exec(&pl, sh);
		if (pipe_fork(&pl) == -1)
			return (-1);
		if (pl.current->pid == 0)
			child_process(&pl, sh);
		else
			parent_line(&pl, sh);
		pl.iter++;
	}
	return (kill_children(&pl, sh));
}
