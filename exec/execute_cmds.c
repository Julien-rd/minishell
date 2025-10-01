/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:40:30 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/01 21:29:39 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cmd_init(t_cmd *cmd)
{
	t_entry	*l_iter;
	size_t	cmd_tokens;
	size_t	cmd_iter;
	size_t	iter;

	l_iter = cmd->line;
	cmd_tokens = 0;
	while (l_iter && l_iter->spec != PIPE)
	{
		if (l_iter->spec == DEFAULT)
		{
			iter = -1;
			while(l_iter->expanded && l_iter->expanded[++iter])
				cmd_tokens++;
		}
		l_iter = l_iter->next;
	}
	cmd->cmd = malloc((cmd_tokens + 1) * sizeof(char *));
	if (!cmd->cmd)
		return (-1);
	l_iter = cmd->line;
	cmd_iter = 0;
	while (l_iter && l_iter->spec != PIPE)
	{
		if (l_iter->spec == DEFAULT)
		{
			iter = -1;
			while(l_iter->expanded && l_iter->expanded[++iter])
				cmd->cmd[cmd_iter++] = l_iter->expanded[iter];
		}
		l_iter = l_iter->next;
	}
	cmd->cmd[cmd_iter] = NULL;
	iter = -1;
	return (0);
}

void	child_exit_handle(t_exec *data, t_cmd *cmd, int errcode)
{
	// free2d(&data->envp.vars);
	// free2d(&data->entries);
	// free(data->input_spec);
	// free(data->pipe_position);
	// free(cmd->cmd);
	if (data->heredoc)
		free2d(&data->heredoc);
	exit(errcode);
}

void	child_process(t_exec *data, t_cmd *cmd)
{
	char	*path;
	int		flag;

	setup_child_signals();;
	if (setup_redirect(data, cmd) == -1)
		child_exit_handle(data, cmd, 1);
	if (cmd->cmd[0] == NULL)
		child_exit_handle(data, cmd, 0);
	if (data->cmd_flag != EXTERNAL)
		builtin_handler(data, cmd);
	path = ft_getpath(data->envp.vars, cmd->cmd[0]); // splitfail malloc
	if (path == NULL || cmd->cmd[0][0] == 0)
		command_fail(path, data, cmd);
	execve(path, cmd->cmd, data->envp.vars);
	execve_fail(path, errno, data, cmd);
}

void	parent_process(t_exec *data)
{
	if (data->pipe_iter != 0)
	{
		if (close(data->prev_fd) == -1)
			exit(1);
	}
	if (data->pipe_iter != data->pipe_count)
	{
		data->prev_fd = data->fd[0];
		if (close(data->fd[1]) == -1)
			exit(1);
	}
}

int	kill_children(t_exec *data)
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
		if (pid == data->last_pid)
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

int	own_cmd_exec(t_exec *data, t_cmd *cmd)
{
	int	flag;

	if (cmd->cmd[0] == NULL)
		return (0);
	flag = options_check(cmd);
	if (data->cmd_flag == CD && !flag)
		return (cd(data, cmd, data->pipe_count));
	if (data->cmd_flag == EXIT)
		return (exit_cmd(data, cmd));
	if (data->cmd_flag == EXPORT && !flag)
		return (export(cmd->cmd, data));
	if (data->cmd_flag == UNSET && !flag)
		return (unset(cmd->cmd, data));
	return (0);
}
void find_start(t_cmd	*cmd, t_exec *data)
{
	t_entry	*l_iter;
	size_t iter;
	
	iter = 0;
	l_iter = data->entries;
	while(l_iter && iter < data->pipe_position[data->pipe_iter])
	{
		l_iter = l_iter->next;
		iter++;
	}
	cmd->line = l_iter;
}

int	execute_cmds(t_exec *data)
{
	t_cmd	cmd;
	t_entry	*iter;

	while (data->pipe_iter <= data->pipe_count)
	{
		find_start(&cmd, data);
		if (cmd_init(&cmd) == -1)
			return (perror("cmd_init"), -1);
		if (data->pipe_iter != data->pipe_count)
			if (pipe(data->fd) == -1)
				return (perror("pipe"), free(cmd.cmd), -1);
		cmd_flag(data, &cmd);
		data->internal_errcode = own_cmd_exec(data, &cmd);
		data->pid = fork();
		if (data->pid == -1)
			return (perror("fork"), free(cmd.cmd), -1);
		if (data->pipe_iter == data->pipe_count)
			data->last_pid = data->pid;
		if (data->pid == 0)
			child_process(data, &cmd);
		else
			parent_process(data);
		data->pipe_iter++;
		free(cmd.cmd);
	}
	return (kill_children(data));
}
