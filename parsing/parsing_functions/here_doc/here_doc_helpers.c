/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 11:09:35 by jromann           #+#    #+#             */
/*   Updated: 2025/10/22 15:04:35 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	hdoc_init(t_sh *sh)
{
	t_entry	*iter;
	int		iter2;

	iter = sh->entries;
	sh->hd_count = 0;
	while (iter != NULL)
	{
		if (iter->spec == HERE_DOC)
			sh->hd_count++;
		iter = iter->next;
	}
	if (sh->hd_count == 0)
	{
		sh->heredoc_fd = NULL;
		return (0);
	}
	sh->heredoc_fd = malloc(sizeof(int) * sh->hd_count);
	if (!sh->heredoc_fd)
		return (-1);
	iter2 = -1;
	while (++iter2 < (int)sh->hd_count)
		sh->heredoc_fd[iter2] = -1;
	return (0);
}

int	ft_lseek(t_sh *sh, size_t hdoc_iter)
{
	if (close(sh->heredoc_fd[hdoc_iter]) == -1)
		return (sh->heredoc_fd[hdoc_iter] = -1, -1);
	sh->heredoc_fd[hdoc_iter] = open(sh->hd_path, O_RDONLY);
	if (sh->heredoc_fd[hdoc_iter] == -1)
		return (perror(sh->hd_path), -1);
	if (safe_unlink(sh->hd_path) == -1)
	{
		free(sh->hd_path);
		sh->hd_path = NULL;
		return (-1);
	}
	free(sh->hd_path);
	sh->hd_path = NULL;
	return (0);
}

char	*heredoc_path(size_t hdoc_iter)
{
	char	*id_str;
	char	*result;

	id_str = ft_itoa(hdoc_iter);
	if (!id_str)
		return (NULL);
	result = ft_strjoin("/tmp/.minishell_heredoc_", id_str);
	free(id_str);
	return (result);
}

int	expand_hdoc_entry(t_entry *buf, t_sh *sh, int expand_flag)
{
	char	*tmp;

	if (expand_flag == 0)
		return (0);
	tmp = expand(buf, sh, HERE_DOC);
	if (!tmp)
		return (-1);
	free(buf->raw_entry);
	buf->raw_entry = tmp;
	return (0);
}

int	safe_unlink(char *path)
{
	if (path == NULL)
		return (-1);
	if (unlink(path) == -1)
		return (perror(path), -1);
	return (0);
}
