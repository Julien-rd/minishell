/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 11:23:11 by jromann           #+#    #+#             */
/*   Updated: 2025/09/17 16:43:21 by eprottun         ###   ########.fr       */
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

static char	*hdoc_entry(char *entry)
{
	size_t	iter;
	int		delimiter_detected;
	char	*buf;
	char	*hdoc_entry;

	delimiter_detected = 0;
	iter = 0;
	if (entry == NULL)
		return (NULL);
	hdoc_entry = calloc(1, 1);
	while (!delimiter_detected && hdoc_entry)
	{
		buf = readline("> ");
		if (!buf)
			return (NULL);
		iter = skip_whitspaces(buf);
		delimiter_detected = ft_strcmp(&buf[iter], entry);
		if (!delimiter_detected)
			hdoc_entry = ft_strjoin(hdoc_entry, buf);
		free(buf);
	}
	return (hdoc_entry);
}

char	**here_doc(t_input *data, size_t heredoc_count)
{
	char	**hdoc_input;
	size_t	iter;
	size_t	hdoc_iter;

	hdoc_iter = 0;
	iter = 0;
	hdoc_input = malloc(sizeof(char *) * heredoc_count);
	while (data->input_spec[iter])
	{
		if (data->input_spec[iter] == HERE_DOC)
		{
			hdoc_input[hdoc_iter] = hdoc_entry(data->entries[iter + 1]);
			// if(!heredoc_input[herehdoc_iter])
			// free; TODO
			hdoc_iter++;
		}
		iter++;
	}
	return (hdoc_input);
}
