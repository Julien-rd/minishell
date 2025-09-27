/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 18:43:33 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/27 11:56:46 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_init(t_exec *data)
{
	if (data->pipe_iter != 0)
	{
		if (dup2(data->prev_fd, 0) == -1)
			return (perror("dup2"), -1);
		if (close(data->prev_fd) == -1)
			return (perror("close"), -1);
	}
	if (data->pipe_iter != data->pipe_count)
	{
		if (dup2(data->fd[1], 1) == -1)
			return (perror("dup2"), -1);
		if (close(data->fd[0]) == -1)
			return (perror("close"), -1);
		if (close(data->fd[1]) == -1)
			return (perror("close"), -1);
	}
	return (0);
}

int	infile_init(char *file_name)
{
	int	fd_in;

	fd_in = open(file_name, O_RDONLY, 0644);
	if (fd_in == -1)
		return (perror(file_name), -1);
	if (dup2(fd_in, 0) == -1)
		return (perror(file_name), -1);
	if (close(fd_in) == -1)
		return (perror(file_name), -1);
	return (0);
}

int	heredoc_init(t_exec *data)
{
	int	fd_heredoc[2];

	if (pipe(fd_heredoc) == -1)
		return (perror("pipe"), -1);
	write(fd_heredoc[1], data->heredoc[data->hdoc_iter],
		ft_strlen(data->heredoc[data->hdoc_iter]));
	if (close(fd_heredoc[1]) == -1)
		return (perror("close"), -1);
	if (dup2(fd_heredoc[0], 0) == -1)
		return (perror("dup2"), -1);
	if (close(fd_heredoc[0]) == -1)
		return (perror("close"), -1);
	data->hdoc_iter++;
	return (0);
}

int	outfile_init(char *file_name, int flag)
{
	int	fd_out;

	fd_out = open(file_name, O_WRONLY | O_CREAT | (flag == OUTFILE)
			* O_TRUNC + (flag == APPEND_FILE) * O_APPEND, 0644);
	if (fd_out == -1)
		return (perror(file_name), -1);
	if (dup2(fd_out, 1) == -1)
		return (perror("dup2"), -1);
	if (close(fd_out) == -1)
		return (perror(file_name), -1);
	return (0);
}

int	setup_redirect(t_exec *data, t_cmd *cmd)
{
	size_t	iter;

	iter = 0;
	if (pipe_init(data) == -1)
		return (-1);
	while (cmd->line_spec[iter] != END && cmd->line_spec[iter] != PIPE)
	{
		if (cmd->line_spec[iter] == INFILE)
			if (infile_init(cmd->line[iter]) == -1)
				return (-1);
		if (cmd->line_spec[iter] == HERE_DOC)
			if (heredoc_init(data) == -1)
				return (-1);
		if (cmd->line_spec[iter] == OUTFILE)
			if (outfile_init(cmd->line[iter], OUTFILE) == -1)
				return (-1);
		if (cmd->line_spec[iter] == APPEND_FILE)
			if (outfile_init(cmd->line[iter], APPEND_FILE) == -1)
				return (-1);
		iter++;
	}
	return (0);
}
