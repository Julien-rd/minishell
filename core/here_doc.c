/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 11:23:11 by jromann           #+#    #+#             */
/*   Updated: 2025/09/28 17:20:25 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	skip_whitspaces(char *buf)
{
	size_t	iter;

	iter = 0;
	while ((buf[iter] >= 9 && buf[iter] <= 13) || buf[iter] == 32)
		iter++;
	return (iter);
}

static int	hdoc_signal_kill(char *buf, char *entry)
{
	if (buf)
		free(buf);
	if (g_current_signal != 2)
	if (g_current_signal != 2)
	{
		write(2, "warning: here-document delimited by end-of-file (wanted '",
			57);
		write(2, entry, ft_strlen(entry));
		write(2, "')\n", 3);
		setup_main_signals();
		return (0);
	}
	setup_main_signals();
	return (-1);
}

static int	hdoc_entry(char *entry, t_exec *data, size_t hdoc_iter)
{
	char	*buf;
	char	*tmp_str;

	setup_heredoc_signals();
	if (entry == NULL)
		return (1);
	data->heredoc[hdoc_iter] = ft_calloc(1, 1);
	if (!data->heredoc[hdoc_iter])
		return (-1);
	while (1)
	{
		buf = readline("> ");
		if (g_current_signal != 0 || !buf)
		if (g_current_signal != 0 || !buf)
			return (hdoc_signal_kill(buf, entry));
		if (ft_strcmp(&buf[skip_whitspaces(buf)], entry) == 0)
			return (free(buf), 0);
		tmp_str = ft_strjointhree(data->heredoc[hdoc_iter], buf, "\n");
		if (!tmp_str)
			return (perror("malloc"), free(buf), -1);
		free(data->heredoc[hdoc_iter]);
		data->heredoc[hdoc_iter] = tmp_str;
		free(buf);
	}
	setup_main_signals();
	return (0);
}

size_t	operator_count(t_exec *data)
{
	size_t	iter;
	size_t	hdoc_count;

	iter = 0;
	hdoc_count = 0;
	while (data->input_spec[iter] != END)
	{
		if (data->input_spec[iter] == HERE_DOC)
			hdoc_count++;
		if (data->input_spec[iter] == PIPE)
			data->pipe_count++;
		iter++;
	}
	if (hdoc_count == 0)
		data->heredoc = NULL;
	return (hdoc_count);
}

int	here_doc(t_exec *data)
{
	size_t	iter;
	size_t	hdoc_iter;
	size_t	hdoc_count;

	hdoc_iter = 0;
	iter = 0;
	hdoc_count = operator_count(data);
	if (hdoc_count == 0)
		return (0);
	data->heredoc = ft_calloc(hdoc_count + 1, sizeof(char *));
	if (!data->heredoc)
		return (-1);
	while (data->input_spec[iter] != END)
	{
		if (data->input_spec[iter] == HERE_DOC)
		{
			if (hdoc_entry(data->entries[iter], data, hdoc_iter) == -1)
				return (free2d(&data->heredoc), -1);
			hdoc_iter++;
		}
		iter++;
	}
	return (0);
}
