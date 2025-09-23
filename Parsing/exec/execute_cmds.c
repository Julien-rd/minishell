/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:40:30 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/23 19:07:16 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	cmd_init(t_cmd *cmd)
{
	size_t	iter;
	size_t	cmd_tokens;
	size_t	cmd_iter;

	iter = 0;
	cmd_tokens = 0;
	while (cmd->line_spec[iter] != END && cmd->line_spec[iter] != PIPE)
	{
		if (cmd->line_spec[iter] == DEFAULT)
			cmd_tokens++;
		iter++;
	}
	cmd->cmd = malloc((cmd_tokens + 1) * sizeof(char *));
	if (!cmd->cmd)
		return (-1);
	iter = 0;
	cmd_iter = 0;
	while (cmd->line_spec[iter] != END && cmd->line_spec[iter] != PIPE)
	{
		if (cmd->line_spec[iter] == DEFAULT)
			cmd->cmd[cmd_iter++] = cmd->line[iter];
		iter++;
	}
	cmd->cmd[cmd_iter] = NULL;
	return (0);
}

void	child_exit_handle(t_exec *data, t_cmd *cmd, int errcode)
{
	free2d(data->envp);
	free2d(data->entries);
	free(data->input_spec);
	free(data->pipe_position);
	free(cmd->cmd);
	if (data->heredoc)
		free(data->heredoc);
	exit(errcode);
}

void	child_process(t_exec *data, t_cmd *cmd)
{
	char	*path;
	int		flag;

	if (setup_redirect(data, cmd) == -1)
		child_exit_handle(data, cmd, 1);
	if (cmd->cmd[0] == NULL)
		child_exit_handle(data, cmd, 0);
	if (data->cmd_flag != EXTERNAL)
	{
		flag = options_check(cmd);
		if (data->cmd_flag == ECHO)
			echo(data, cmd, flag);
		else if (data->cmd_flag == PWD && flag != 1)
			pwd(data, cmd);
		else if (data->cmd_flag == ENV && flag != 1)
			env(data->envp, data, cmd);
		else
		{
			internal_cmd_error(data, cmd);
			// ERRORHANDLING für exit etc. !NICHT IN PARENTPROCESS
			// exit(data->internal_errcode);
		}
	}
	//splitfail malloc
	path = ft_getpath(data->envp, cmd->cmd[0]);
	if (path == NULL)
	{
		perror(cmd->cmd[0]);
		child_exit_handle(data, cmd, 1);
	}
	execve(path, cmd->cmd, data->envp);
	child_exit_handle(data, cmd, 1);
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
	if (!flag)
	{
		if (data->cmd_flag == CD)
			return (cd(data, cmd, data->pipe_count));
		if (data->cmd_flag == EXIT)
			return (exit_cmd(data, cmd));
		if (data->cmd_flag == EXPORT)
			return (export(cmd->cmd, data));
		if (data->cmd_flag == UNSET)
			return (unset(cmd->cmd, data));
	}
	return (0);
}

int	execute_cmds(t_exec *data)
{
	t_cmd	cmd;

	data->hdoc_iter = 0;
	data->pipe_iter = 0;
	while (data->pipe_iter <= data->pipe_count)
	{
		cmd.line = &data->entries[data->pipe_position[data->pipe_iter]];
		cmd.line_spec = &data->input_spec[data->pipe_position[data->pipe_iter]];
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
