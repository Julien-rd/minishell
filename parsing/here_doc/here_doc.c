/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 11:23:11 by jromann           #+#    #+#             */
/*   Updated: 2025/10/09 16:10:43 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expand_hdoc_entry(t_entry *buf, t_sh *sh, int expand_flag)
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

static int	hdoc_mode(t_sh *sh, int expand_flag, char *delimiter,
		size_t hdoc_iter)
{
	t_entry	buf;
	char	*tmp_str;

	while (1)
	{
		buf.raw_entry = readline("> ");
		if (g_current_signal != 0 || !buf.raw_entry)
			return (hdoc_signal_kill(buf.raw_entry, delimiter));
		if (ft_strcmp(&buf.raw_entry[skip_whitspaces(buf.raw_entry)],
				delimiter) == 0)
			return (setup_main_signals(sh), free(buf.raw_entry), 0);
		if (expand_hdoc_entry(&buf, sh, expand_flag) == -1)
			return (setup_main_signals(sh), free(buf.raw_entry), -1);
		tmp_str = ft_strjointhree(sh->heredoc[hdoc_iter], buf.raw_entry,
				"\n");
		if (!tmp_str)
			return (perror("malloc"), setup_main_signals(sh), free(buf.raw_entry),
				-1);
		free(sh->heredoc[hdoc_iter]);
		sh->heredoc[hdoc_iter] = tmp_str;
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
	setup_heredoc_signals();
	sh->heredoc[hdoc_iter] = ft_calloc(1, 1);
	if (!sh->heredoc[hdoc_iter])
		return (setup_main_signals(sh), -1);
	delimiter = remove_quotes(iter->raw_entry, ft_strlen(iter->raw_entry));
	if (!delimiter)
		return (-1);
	if (ft_strncmp(delimiter, iter->raw_entry, ft_strlen(delimiter) + 1) == 0)
		expand_flag = 1;
	if (hdoc_mode(sh, expand_flag, delimiter, hdoc_iter) == -1)
		return (setup_main_signals(sh), free(delimiter), -1);
	return (setup_main_signals(sh), free(delimiter), 0);
}

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
		sh->heredoc = NULL;
	return (hdoc_count);
}

int	here_doc(t_sh *sh)
{
	t_entry	*iter;
	size_t	hdoc_iter;
	size_t	hdoc_count;

	hdoc_iter = 0;
	iter = sh->entries;
	hdoc_count = operator_count(sh);
	if (hdoc_count == 0)
		return (0);
	sh->heredoc = ft_calloc(hdoc_count + 1, sizeof(char *));
	if (!sh->heredoc)
		return (-1);
	while (iter != NULL)
	{
		if (iter->spec == HERE_DOC)
		{
			if (hdoc_entry(iter, sh, hdoc_iter) == -1)
				return (free2d(&sh->heredoc), -1);
			hdoc_iter++;
		}
		iter = iter->next;
	}
	return (0);
}
