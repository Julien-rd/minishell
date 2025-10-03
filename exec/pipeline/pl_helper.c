/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pl_helper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:08:27 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/03 10:33:26 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_pipes(t_sh *sh)
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

int	init_pipe_pos(t_pipeline *pl, t_sh *sh)
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

void	find_start(t_pipeline *pl, t_sh *sh, size_t cmd_iter)
{
	t_entry	*node;
	size_t	iter;

	iter = 0;
	node = sh->entries;
	while (node && iter < pl->position[cmd_iter])
	{
		node = node->next;
		iter++;
	}
	pl->cmds[cmd_iter].line = node;
}

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
			while (node->expanded && node->expanded[++iter])
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
			while (node->expanded && node->expanded[++iter])
				current->argv[cmd_iter++] = node->expanded[iter];
		}
		node = node->next;
	}
	current->argv[cmd_iter] = NULL;
	iter = -1;
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
