/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:40:30 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/19 15:03:07 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int setup_redirect(t_exec *data, t_cmd *cmd)
{
	size_t iter;
	size_t in_count;
	size_t out_count;
	int fd_in;
	int fd_write;
	int fd_take;
	int fd_fdout;
	int fd_fdin;
	int	fd_out;

	in_count = 0;
	out_count = 0;
	iter = 0;
	if (data->pipe_iter != 0)
	{
		if (dup2(data->prev_fd, 0) == -1)
			return (1);
		if (close(data->prev_fd) == -1)
			return (1);
	}
	if (data->pipe_iter != data->pipe_count)
	{
		if (dup2(data->fd[1], 1) == -1)
			return (1);
		if (close(data->fd[0]) == -1)
			return (1);
		if (close(data->fd[1]) == -1)
			return (1);
	}
	while (cmd->line_spec[iter] != END && cmd->line_spec[iter] != PIPE)
	{
		if (cmd->line_spec[iter] == INFILE)
		{
			if (in_count != 0)
				close(fd_in);
			fd_in = open(cmd->line[iter], O_RDONLY, 0644);
			if (fd_in == -1)
				perror(cmd->line[iter]), exit(1);
			if (dup2(fd_in, 0) == -1)
				perror(cmd->line[iter]), exit(1);
			in_count++;
		}
		if (cmd->line_spec[iter] == HERE_DOC)
		{
			// write(1, data->heredoc[data->hdoc_iter], ft_strlen(data->heredoc[data->hdoc_iter]));
			if (in_count != 0)
				close(fd_in);
			int fd_heredoc[2];
			pipe(fd_heredoc);
			write(fd_heredoc[1], data->heredoc[data->hdoc_iter], ft_strlen(data->heredoc[data->hdoc_iter]));
			dup2(fd_heredoc[0], 0);
			data->hdoc_iter++;
			close(fd_heredoc[1]);
		}
		// if (cmd->line_spec[iter] == FD_WRITE)
		// {
		// 	fd_write = ft_atoi(cmd->line[iter]);
		// 	iter += 2;
		// 	fd_fdout = open(cmd->line[iter], O_RDONLY, 0644);
		// 	if (dup2(fd_write, fd_fdout) == -1)
		// 		exit(1);
		// }
		// if (cmd->line_spec[iter] == FD_TAKE)
		// {
		// 	fd_write = ft_atoi(cmd->line[iter]);
		// 	iter += 2;
		// 	fd_fdout = open(cmd->line[iter], O_RDONLY, 0644);
		// 	if (dup2(fd_fdout, fd_write) == -1)
		// 		exit(1);
		// }
		if (cmd->line_spec[iter] == OUTFILE)
		{
			if (out_count != 0)
				close(fd_out);
			fd_out = open(cmd->line[iter], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd_out == -1)
				exit(1);
			if (dup2(fd_out, 1) == -1)
				exit(1);
			out_count++;
		}
		if (cmd->line_spec[iter] == APPEND_FILE)
		{
			if (out_count != 0)
				close(fd_out);
			fd_out = open(cmd->line[iter], O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd_out == -1)
				exit(1);
			if (dup2(fd_out, 1) == -1)
				exit(1);
			out_count++;
		}
		iter++;
	}
	return (0);
}

int cmd_init(t_cmd *cmd)
{
	size_t iter;
	size_t cmd_tokens;
	size_t cmd_iter;

	iter = 0;
	while (cmd->line_spec[iter] != END && cmd->line_spec[iter] != PIPE)
	{
		if (cmd->line_spec[iter] == DEFAULT)
			cmd_tokens++;
		iter++;
	}
	cmd->cmd = malloc((cmd_tokens + 1) * sizeof(char *));
	iter = 0;
	cmd_iter = 0;
	while (cmd->line_spec[iter] != END && cmd->line_spec[iter] != PIPE)
	{
		if (cmd->line_spec[iter] == DEFAULT)
			cmd->cmd[cmd_iter++] = cmd->line[iter];
		iter++;
	}
	cmd->cmd[cmd_iter] = NULL;
}

void child_process(t_exec *data)
{
	char *path;
	t_cmd cmd;

	cmd.line = &data->entries[data->pipe_position[data->pipe_iter]];
	cmd.line_spec = &data->input_spec[data->pipe_position[data->pipe_iter]];
	cmd_init(&cmd);
	// redirection of files
	if (setup_redirect(data, &cmd))
		exit(1);
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
		{
			// ft_error_handle(cmds, "close", PIPE_DEP, YES);
			exit(1);
		}
	}
	if (data->pipe_iter != data->pipe_count)
	{
		data->prev_fd = data->fd[0];
		if (close(data->fd[1]) == -1)
		{
			// ft_error_handle(cmds, "close", PIPE_READ, YES);
			exit(1);
		}
	}
}

int	ft_kill_children(t_exec *data)
{
	int		status;
	int		success;
	pid_t	pid;

	success = 0;
	data->return_value = 1;
	while (1)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
		{
			if (errno == ECHILD && ++success)
				break ;
			else if (errno == EINTR)
				continue ;
			else
				break ;
		}
		if (pid == data->last_pid)
			if (WIFEXITED(status))
				data->return_value = WEXITSTATUS(status);
	}
	if (errno != ECHILD)
		perror("waitpid");
	return (success);
}

int execute_cmds(t_exec *data)
{
	data->pipe_iter = 0;
	data->hdoc_iter = 0;
	while (data->pipe_iter <= data->pipe_count)
	{
		if (data->pipe_iter != data->pipe_count)
			if (pipe(data->fd) == -1)
				return (perror("pipe"), 1);
		data->pid = fork();
		if (data->pid == -1)
			return (1);
		if (data->pipe_iter == data->pipe_count)
			data->last_pid = data->pid;
		if (data->pid == 0)
			child_process(data);
		else
			parent_process(data);
		data->pipe_iter++;
	}
	if (ft_kill_children(data))
		return (data->return_value);
	return (1);
}
