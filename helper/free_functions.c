/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 14:47:39 by jromann           #+#    #+#             */
/*   Updated: 2025/10/14 16:11:56 by eprottun         ###   ########.fr       */
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
	if(sh->og_path)
		free(sh->og_path);
	if (sh->heredoc)
		free2d(&sh->heredoc);
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

void	cleanup(t_sh *sh)
{
	free_list(sh->entries);
	free2d(&sh->heredoc);
}
