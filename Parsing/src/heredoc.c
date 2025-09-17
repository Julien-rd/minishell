/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 11:23:11 by jromann           #+#    #+#             */
/*   Updated: 2025/09/17 12:22:05 by jromann          ###   ########.fr       */
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

char	*heredoc(t_input *data, int heredoc_pos)
{
	char	*heredoc_input;
	char	*buf;
	char	*delimiter;
	size_t	iter;
	size_t	delimiter_size;

	heredoc_input = calloc(1, 1);
	if (data->entries[heredoc_pos + 1] == NULL || !heredoc_input)
		return (NULL);
	delimiter = data->entries[heredoc_pos + 1];
    iter = skip_whitspaces(buf);
	buf = ft_strlen(buf);
	while (ft_strcmp(buf, delimiter))
	{
		buf = readline("> ");
		if (!buf)
			return (NULL); // FREE HEREDOC;
		heredoc_input = ft_strjoin(heredoc_input, buf);
		free(buf);
		first_iter = 0;
	}
	return (heredoc_input);
}
