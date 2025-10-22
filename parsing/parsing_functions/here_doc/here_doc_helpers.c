/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 11:09:35 by jromann           #+#    #+#             */
/*   Updated: 2025/10/22 11:13:05 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	operator_count(t_sh *sh)
{
	size_t	hdoc_count;
	t_entry	*iter;

	iter = sh->entries;
	hdoc_count = 0;
	while (iter != NULL)
	{
		if (iter->spec == HERE_DOC)
			hdoc_count++;
		iter = iter->next;
	}
	if (hdoc_count == 0)
		sh->heredoc_fd = NULL;
	return (hdoc_count);
}

int	ft_lseek(t_sh *sh, size_t hdoc_iter)
{
	if (close(sh->heredoc_fd[hdoc_iter]) == -1)
		return (sh->heredoc_fd[hdoc_iter] = -1, -1);
	if (open(sh->hd_path, O_RDWR) == -1)
		return (-1);
	if (safe_unlink(sh->hd_path) == -1)
		return (perror("ft_lseek"), -1);
	free(sh->hd_path);
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
		return (-1);
	return (0);
}
