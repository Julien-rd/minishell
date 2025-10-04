/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 14:08:37 by jromann           #+#    #+#             */
/*   Updated: 2025/10/04 14:13:39 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	setup_pipe_input(t_pipeline *pl)
{
	if (pl->iter == 0)
		return (0);
	if (dup2(pl->prev_fd, STDIN_FILENO) == -1)
		return (perror("dup2"), -1);
	if (close(pl->prev_fd) == -1)
		return (perror("close"), -1);
	return (0);
}

static int	setup_pipe_output(t_pipeline *pl)
{
	if (pl->iter == pl->count)
		return (0);
	if (dup2(pl->fd[1], STDOUT_FILENO) == -1)
		return (perror("dup2"), close(pl->fd[0]), close(pl->fd[1]), -1);
	if (close(pl->fd[0]) == -1 || close(pl->fd[1]) == -1)
		return (perror("close"), -1);
	return (0);
}

int	pipe_init(t_pipeline *pl)
{
	if (setup_pipe_input(pl) == -1)
	{
		if (pl->iter != pl->count)
			return (close(pl->fd[0]), close(pl->fd[1]), -1);
		return (-1);
	}
	return (setup_pipe_output(pl));
}
