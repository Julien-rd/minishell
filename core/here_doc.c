/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 11:23:11 by jromann           #+#    #+#             */
/*   Updated: 2025/09/27 15:25:47 by jromann          ###   ########.fr       */
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

static int	hdoc_entry(char *entry, t_exec *data, size_t hdoc_iter)
{
	size_t	iter;
	int		delimiter_not_detected;
	char	*buf;
	char	*tmp_str;

	delimiter_not_detected = 1;
	setup_heredoc_signals();
	iter = 0;
	if (entry == NULL)
		return (1);
	data->heredoc[hdoc_iter] = ft_calloc(1, 1);
	if (!data->heredoc[hdoc_iter])
		return (-1);
	while (delimiter_not_detected)
	{
		buf = readline("> ");
		if (current_signal != 0 || !buf)
		{
			if (buf)
				free(buf);
			if (current_signal != 2)
			{
				write(2,
					"warning: here-document delimited by end-of-file (wanted '",
					57);
				write(2, entry, ft_strlen(entry));
				write(2, "')\n", 3);
				setup_main_signals();
				return (0);
			}
			setup_main_signals();
			return (-1);
		}
		iter = skip_whitspaces(buf);
		delimiter_not_detected = ft_strcmp(&buf[iter], entry);
		if (delimiter_not_detected)
		{
			tmp_str = ft_strjointhree(data->heredoc[hdoc_iter], buf, "\n");
			if (!tmp_str)
				return (perror("malloc"), free(buf), -1);
			free(data->heredoc[hdoc_iter]);
			data->heredoc[hdoc_iter] = tmp_str;
		}
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
	{
		data->heredoc = NULL;
		return (0);
	}
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
