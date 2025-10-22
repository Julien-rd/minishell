/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 11:23:11 by jromann           #+#    #+#             */
/*   Updated: 2025/10/22 14:55:07 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	hdoc_mode(t_sh *sh, int expand_flag, char *delimiter,
		size_t hdoc_iter)
{
	t_entry	buf;

	sh->hd_path = heredoc_path(hdoc_iter);
	if (!sh->hd_path)
		return (setup_main_signals(sh), -1);
	sh->heredoc_fd[hdoc_iter] = open(sh->hd_path, O_CREAT | O_RDWR | O_TRUNC,
			0600);
	if (sh->heredoc_fd[hdoc_iter] == -1)
		return (perror(sh->hd_path), -1);
	while (1)
	{
		buf.raw_entry = readline("> ");
		if (g_current_signal != 0 || !buf.raw_entry)
			return (hdoc_signal_kill(sh, buf.raw_entry, delimiter));
		if (ft_strcmp(buf.raw_entry, delimiter) == 0)
			return (setup_main_signals(sh), free(buf.raw_entry), 0);
		if (expand_hdoc_entry(&buf, sh, expand_flag) == -1)
			return (setup_main_signals(sh), free(buf.raw_entry), -1);
		if (safe_write(sh->heredoc_fd[hdoc_iter], buf.raw_entry,
				ft_strlen(buf.raw_entry)) == -1)
			return (setup_main_signals(sh), free(buf.raw_entry), -1);
		if (1 || safe_write(sh->heredoc_fd[hdoc_iter], "\n", 1) == -1)
			return (setup_main_signals(sh), free(buf.raw_entry), -1);
		free(buf.raw_entry);
	}
}

static int	hdoc_entry(t_entry *iter, t_sh *sh, size_t hdoc_iter)
{
	char	*delimiter;
	int		expand_flag;

	expand_flag = 0;
	if (iter->raw_entry == NULL)
		return (-1);
	delimiter = remove_quotes(iter->raw_entry, NULL,
			ft_strlen(iter->raw_entry));
	if (!delimiter)
		return (-1);
	if (ft_strncmp(delimiter, iter->raw_entry, ft_strlen(delimiter) + 1) == 0)
		expand_flag = 1;
	if (hdoc_mode(sh, expand_flag, delimiter, hdoc_iter) == -1)
		return (setup_main_signals(sh), free(delimiter), -1);
	return (setup_main_signals(sh), free(delimiter), 0);
}

int	here_doc(t_sh *sh)
{
	t_entry	*iter;
	size_t	hdoc_iter;

	hdoc_iter = 0;
	iter = sh->entries;
	sh->hd_path = NULL;
	if (hdoc_init(sh) == -1)
		return (-1);
	if (!sh->heredoc_fd)
		return (0);
	while (iter != NULL)
	{
		if (iter->spec == HERE_DOC)
		{
			setup_heredoc_signals();
			if (hdoc_entry(iter, sh, hdoc_iter) == -1 || ft_lseek(sh,
					hdoc_iter) == -1)
				return (close_fd(sh), safe_unlink(sh->hd_path),
					free(sh->heredoc_fd), free(sh->hd_path), -1);
			hdoc_iter++;
		}
		iter = iter->next;
	}
	return (0);
}
