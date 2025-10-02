/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_central.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 19:10:38 by jromann           #+#    #+#             */
/*   Updated: 2025/10/02 12:00:24 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	count_pipes(t_sh *sh)
{
	t_entry	*node;

	node = sh->entries;
	while (node != NULL)
	{
		if (node->spec == PIPE)
			sh->pipe.count++;
		node = node->next;
	}
}
static int	init_pipe_pos(t_sh *sh)
{
	t_entry	*node;
	size_t	counter;
	size_t	iter;

	iter = 0;
	count_pipes(sh);
	sh->pipe.position = malloc(sizeof(int) * (sh->pipe.count + 1));
	if (!sh->pipe.position)
		return (-1);
	sh->pipe.position[iter++] = 0;
	counter = 0;
	node = sh->entries;
	while (node != NULL)
	{
		if (node->spec == PIPE)
			sh->pipe.position[iter++] = counter + 1;
		counter++;
		node = node->next;
	}
	return (0);
}

static int	init_sh(t_sh *sh)
{
	sh->pipe.count = 0;
	sh->exit = 0;
	sh->pipe.hdoc_iter = 0;
	sh->pipe.iter = 0;
	if (init_pipe_pos(sh) == -1)
		return (free2d(&sh->heredoc), free2d(&sh->envp.vars), -1);
	return (0);
}

int	exec_central(t_sh *sh)
{
	int		exit_code;

	exit_code = 0;
	// return -1;
	if (init_sh(sh) == -1)
		return (free_list(sh->entries), -1);
	exit_code = execute_cmds(sh);
	free_list(sh->entries);
	free(sh->pipe.position);
	if(sh->heredoc)
		free2d(&sh->heredoc);
	if (exit_code == -1)
		free2d(&sh->envp.vars);
	return (exit_code);
}
