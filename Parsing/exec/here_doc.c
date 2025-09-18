/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 11:23:11 by jromann           #+#    #+#             */
/*   Updated: 2025/09/18 18:14:32 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	int		first_round;

	delimiter_not_detected = 1;
	iter = 0;
	if (entry == NULL)
		return (1);
	data->heredoc[hdoc_iter] = calloc(1, 1);
	if (!data->heredoc[hdoc_iter])
		return (1);
	while (delimiter_not_detected && data->heredoc[hdoc_iter])
	{
		buf = readline("> ");
		if (!buf)
			return (1);
		iter = skip_whitspaces(buf);
		delimiter_not_detected = ft_strcmp(&buf[iter], entry);
		if (delimiter_not_detected)
		{
			if (!first_round)
				data->heredoc[hdoc_iter] = ft_strjoin(data->heredoc[hdoc_iter],
						"\n");
			data->heredoc[hdoc_iter] = ft_strjoin(data->heredoc[hdoc_iter],
					buf);
			first_round = 0;
		}
		free(buf);
	}
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
	return (iter);
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
	data->heredoc = malloc(sizeof(char *) * hdoc_count);
	while (data->input_spec[iter] != END)
	{
		if (data->input_spec[iter] == HERE_DOC)
		{
			hdoc_entry(data->entries[iter], data, hdoc_iter);
			if (!data->heredoc[hdoc_iter])
				return (1);
			hdoc_iter++;
		}
		iter++;
	}
	return (0);
}
