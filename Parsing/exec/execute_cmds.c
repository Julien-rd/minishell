/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:40:30 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/20 19:09:37 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	cmd_init(t_cmd *cmd)
{
	size_t	iter;
	size_t	cmd_tokens;
	size_t	cmd_iter;

	iter = 0;
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

void	child_process(t_exec *data)
{
	char	*path;
	t_cmd	cmd;

	cmd.line = &data->entries[data->pipe_position[data->pipe_iter]];
	cmd.line_spec = &data->input_spec[data->pipe_position[data->pipe_iter]];
	if (cmd_init(&cmd) == -1)
		exit(1);
	if (setup_redirect(data, &cmd) == -1)
		exit(1);
	if (!ft_strncmp(cmd.cmd[0], "exit", 5) && cmd.cmd[1] == NULL && !data->pipe_count)
		exit(12);
	if (!ft_strncmp(cmd.cmd[0], "exit", 5) && cmd.cmd[1] == NULL)
		exit(0);
	if (!ft_strncmp(cmd.cmd[0], "export", 7))
		export(cmd.cmd, data);
	if (!ft_strncmp(cmd.cmd[0], "unset", 6))
		unset(cmd.cmd, data);
	if (!ft_strncmp(cmd.cmd[0], "env", 4))
		env(data->envp);
	path = ft_getpath(data->envp, cmd.cmd[0]);
	if (path == NULL)
	{
		perror(cmd.cmd[0]);
		exit(1);
	}
	execve(path, cmd.cmd, data->envp);
	exit(1);
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

int	execute_cmds(t_exec *data)
{
	data->hdoc_iter = 0;
	data->pipe_iter = 0;
	while (data->pipe_iter <= data->pipe_count)
	{
		if (data->pipe_iter != data->pipe_count)
			if (pipe(data->fd) == -1)
				return (perror("pipe"), -1);
		data->pid = fork();
		if (data->pid == -1)
			return (perror("fork"), -1);
		if (data->pipe_iter == data->pipe_count)
			data->last_pid = data->pid;
		if (data->pid == 0)
			child_process(data);
		else
			parent_process(data);
		data->pipe_iter++;
	}
	return (kill_children(data));
}
