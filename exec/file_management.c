/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 18:43:33 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/01 10:27:16 by jromann          ###   ########.fr       */
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
		return (close(fd_in), perror(file_name), -1);
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

	fd_out = open(file_name, O_WRONLY | O_CREAT | (flag == OUTFILE) * O_TRUNC
			+ (flag == APPEND_FILE) * O_APPEND, 0644);
	if (fd_out == -1)
		return (perror(file_name), -1);
	if (dup2(fd_out, 1) == -1)
		return (close(fd_out), perror("dup2"), -1);
	if (close(fd_out) == -1)
		return (perror(file_name), -1);
	return (0);
}
static int	ambiguous(t_entry *iter)
{
	if (!(iter->spec == INFILE || iter->spec == OUTFILE || iter->spec == APPEND_FILE))
		return (0);
	if (iter->expanded == NULL || (iter->expanded[0] && iter->expanded[1]))
	{
		if (safe_write(2, iter->raw_entry, ft_strlen(iter->raw_entry)) == -1)
			return (-1);
		if (safe_write(2, ": ambiguous redirect\n", 21) == -1)
			return (-1);
		return (1);
	}
	return (0);
}

int	setup_redirect(t_exec *data, t_cmd *cmd)
{
	t_entry	*iter;

	iter = cmd->line;
	fflush(stdout);
	if (pipe_init(data) == -1)
		return (-1);
	while (iter && iter->spec != PIPE)
	{
		if (ambiguous(iter))
			return (-1);
		if (iter->spec == INFILE)
			if (infile_init(iter->expanded[0]) == -1)
				return (-1);
		if (iter->spec == HERE_DOC)
			if (heredoc_init(data) == -1)
				return (-1);
		if (iter->spec == OUTFILE)
			if (outfile_init(iter->expanded[0], OUTFILE) == -1)
				return (-1);
		if (iter->spec == APPEND_FILE)
			if (outfile_init(iter->expanded[0], APPEND_FILE) == -1)
				return (-1);
		iter = iter->next;
	}
	return (0);
}
