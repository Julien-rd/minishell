/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 14:47:39 by jromann           #+#    #+#             */
/*   Updated: 2025/10/22 12:10:14 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free2d(char ***str)
{
	size_t	iter;

	iter = 0;
	if (!(*str))
		return ;
	while ((*str)[iter])
	{
		free((*str)[iter]);
		iter++;
	}
	free(*str);
}

void	free_list(t_entry *list)
{
	t_entry	*tmp;

	while (list)
	{
		if (list->raw_entry)
		{
			free(list->raw_entry);
			list->raw_entry = NULL;
		}
		if (list->expanded)
		{
			free2d(&list->expanded);
			list->expanded = NULL;
		}
		if (list->expand_bool)
			free(list->expand_bool);
		tmp = list->next;
		free(list);
		list = tmp;
	}
}

void	child_exit(t_sh *sh, t_pipeline *pl, int errcode)
{
	free2d(&sh->envp.vars);
	free_list(sh->entries);
	free(pl->position);
	get_next_line(-1);
	free_cmds(pl, pl->count + 1);
	if (sh->og_path)
		free(sh->og_path);
	if (sh->hd_count)
		close_fd(sh);
	free(sh->heredoc_fd);
	exit(errcode);
}

void	free_cmds(t_pipeline *pl, size_t arr_len)
{
	size_t	iter;

	iter = 0;
	while (iter < arr_len)
	{
		free(pl->cmds[iter].argv);
		iter++;
	}
	free(pl->cmds);
}

void	close_fd(t_sh *sh)
{
	size_t	iter;

	iter = 0;
	while (iter < sh->hd_count)
	{
		if (sh->heredoc_fd[iter] != -1)
			close(sh->heredoc_fd[iter]);
		sh->heredoc_fd[iter] = -1;
		iter++;
	}
}
