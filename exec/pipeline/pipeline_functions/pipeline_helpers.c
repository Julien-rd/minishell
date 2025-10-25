/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:27:12 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/25 14:40:19 by eprottun         ###   ########.fr       */
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
		while (cur->argv[iter] && cur->argv[iter][0] == '-'
			&& cur->argv[iter][1])
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

int	pl_cleanup(t_pipeline *pl, t_sh *sh, int flag)
{
	free_cmds(pl, pl->count + 1);
	free(pl->position);
	if (flag == FAILURE)
		return (-1);
	return (kill_children(pl, sh));
}

size_t	hdoc_add(t_pipeline *pl)
{
	t_entry	*node;
	size_t	add;

	add = 0;
	node = pl->current->line;
	while (node && node->spec != PIPE)
	{
		if (node->spec == HERE_DOC)
			add++;
		node = node->next;
	}
	return (add);
}
