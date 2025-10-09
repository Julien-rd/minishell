/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:27:12 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/09 17:01:53 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	options_check(t_cmd *cur)
{
	size_t	iter;
	size_t	n_iter;

	iter = 1;
	if (!cur)
		return (-1);
	if (!ft_strncmp(cur->argv[0], "echo", 5) && cur->argv[1])
	{
		while (cur->argv[iter] && cur->argv[iter][0] == '-')
		{
			n_iter = 1;
			while (cur->argv[iter][n_iter] == 'n')
				n_iter++;
			if (cur->argv[iter][n_iter])
				break ;
			iter++;
		}
		return (iter - 1);
	}
	if (cur->argv[1] != NULL && cur->argv[1][0] == '-' && cur->argv[1][1])
		return (-1);
	return (0);
}

int	cmd_tokens(t_cmd *current)
{
	t_entry	*node;
	size_t	cmd_tokens;
	int		iter;

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
	return (cmd_tokens);
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

int	pl_cleanup(t_pipeline *pl, t_sh *sh, int flag)
{
	free_cmds(pl, pl->count + 1);
	free(pl->position);
	if (flag == FAILURE)
		return (-1);
	return (kill_children(pl, sh));
}
