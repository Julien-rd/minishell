/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 18:43:33 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/22 09:40:38 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	infile_init(char *file_name)
{
	int	fd_in;

	fd_in = open(file_name, O_RDONLY, 0644);
	if (fd_in == -1)
		return (perror(file_name), -1);
	if (dup2(fd_in, 0) == -1)
		return (perror(file_name), close(fd_in), -1);
	if (close(fd_in) == -1)
		return (perror(file_name), -1);
	return (0);
}

static int	heredoc_init(t_pipeline *pl, t_sh *sh)
{
	if (dup2(sh->heredoc_fd[pl->hdoc_iter], 0) == -1)
		return (perror("dup2"), -1);
	pl->hdoc_iter++;
	return (0);
}

static int	outfile_init(char *file_name, int flag)
{
	int	fd_out;

	fd_out = open(file_name, O_WRONLY | O_CREAT | (((flag == OUTFILE) * O_TRUNC)
			+ ((flag == APPEND_FILE) * O_APPEND)), 0644);
	if (fd_out == -1)
		return (perror(file_name), -1);
	if (dup2(fd_out, 1) == -1)
		return (perror("dup2"), close(fd_out), -1);
	if (close(fd_out) == -1)
		return (perror(file_name), -1);
	return (0);
}

static int	ambiguous(t_entry *iter)
{
	if (!(iter->spec == INFILE || iter->spec == OUTFILE
			|| iter->spec == APPEND_FILE))
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

int	setup_redirect(t_sh *sh, t_pipeline *pl)
{
	t_entry	*node;

	node = pl->cmds[pl->iter].line;
	if (pipe_init(pl) == -1)
		return (-1);
	while (node && node->spec != PIPE)
	{
		if (ambiguous(node))
			return (-1);
		if (node->spec == INFILE)
			if (infile_init(node->expanded[0]) == -1)
				return (-1);
		if (node->spec == HERE_DOC)
			if (heredoc_init(pl, sh) == -1)
				return (-1);
		if (node->spec == OUTFILE)
			if (outfile_init(node->expanded[0], OUTFILE) == -1)
				return (-1);
		if (node->spec == APPEND_FILE)
			if (outfile_init(node->expanded[0], APPEND_FILE) == -1)
				return (-1);
		node = node->next;
	}
	return (0);
}
