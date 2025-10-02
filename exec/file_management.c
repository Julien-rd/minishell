/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 18:43:33 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/02 11:36:28 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_init(t_sh *sh)
{
	if (sh->pipe.iter != 0)
	{
		if (dup2(sh->pipe.prev_fd, 0) == -1)
			return (perror("dup2"), -1);
		if (close(sh->pipe.prev_fd) == -1)
			return (perror("close"), -1);
	}
	if (sh->pipe.iter != sh->pipe.count)
	{
		if (dup2(sh->pipe.fd[1], 1) == -1)
			return (perror("dup2"), -1);
		if (close(sh->pipe.fd[0]) == -1)
			return (perror("close"), -1);
		if (close(sh->pipe.fd[1]) == -1)
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

int	heredoc_init(t_sh *sh)
{
	int	fd_heredoc[2];

	if (pipe(fd_heredoc) == -1)
		return (perror("pipe"), -1);
	write(fd_heredoc[1], sh->heredoc[sh->pipe.hdoc_iter],
		ft_strlen(sh->heredoc[sh->pipe.hdoc_iter]));
	if (close(fd_heredoc[1]) == -1)
		return (perror("close"), -1);
	if (dup2(fd_heredoc[0], 0) == -1)
		return (perror("dup2"), -1);
	if (close(fd_heredoc[0]) == -1)
		return (perror("close"), -1);
	sh->pipe.hdoc_iter++;
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

int	setup_redirect(t_sh *sh, t_cmd *cmd)
{
	t_entry	*iter;

	iter = cmd->line;
	if (pipe_init(sh) == -1)
		return (-1);
	while (iter && iter->spec != PIPE)
	{
		if (ambiguous(iter))
			return (-1);
		if (iter->spec == INFILE)
			if (infile_init(iter->expanded[0]) == -1)
				return (-1);
		if (iter->spec == HERE_DOC)
			if (heredoc_init(sh) == -1)
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
