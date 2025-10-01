/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 11:23:11 by jromann           #+#    #+#             */
/*   Updated: 2025/10/01 13:43:41 by jromann          ###   ########.fr       */
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

// static char *extract_delimiter(char *entry, int *expand)
// {
// 	int len;
// 	size_t iter;
// 	char *str;

// 	len = ft_strlen(entry);
// 	str = ft_calloc(sizeof(char), len + 1);
// 	while(entry[iter])
// 	{

// 	}
// 	if (entry[0] == '\"' && entry[1] && entry[len - 1] == '\"')
// 	{
// 		*expand = 1;
// 	}
// 	else
// 	{
// 		*expand = 0;
// 	}
// }

static int	hdoc_entry(t_entry *iter, t_input *data, size_t hdoc_iter)
{
	t_entry menu;
	char	*tmp_str;
	char	*tmp_str2;
	char	*delimiter;
	int		expand_flag;

	expand_flag = 0;
	if (iter->raw_entry == NULL)
		return (1);
	setup_heredoc_signals();
	data->heredoc[hdoc_iter] = ft_calloc(1, 1);
	if (!data->heredoc[hdoc_iter])
		return (setup_main_signals(), -1);
	// delimiter = remove_quotes(iter->raw_entry);
	delimiter = iter->raw_entry;
	if (ft_strncmp(delimiter, iter->raw_entry, ft_strlen(delimiter) + 1) == 0)
		expand_flag = 1;
	while (1)
	{
		menu.raw_entry = readline("> ");
		if (g_current_signal != 0 || !menu.raw_entry)
			if (g_current_signal != 0 || !menu.raw_entry)
				return (hdoc_signal_kill(menu.raw_entry, delimiter));
		if (ft_strcmp(&menu.raw_entry[skip_whitspaces(menu.raw_entry)], delimiter) == 0)
			return (setup_main_signals(), free(menu.raw_entry), 0);
		if (expand_flag == 1)
		{
			tmp_str2 = expand(&menu, data);
			if (!tmp_str2)
				return (free(menu.raw_entry), -1);
			free(menu.raw_entry);
			menu.raw_entry = tmp_str2;
		}
		tmp_str = ft_strjointhree(data->heredoc[hdoc_iter], menu.raw_entry, "\n");
		if (!tmp_str)
			return (perror("malloc"), setup_main_signals(), free(menu.raw_entry), -1);
		free(data->heredoc[hdoc_iter]);
		data->heredoc[hdoc_iter] = tmp_str;
		free(menu.raw_entry);
	}
	return (setup_main_signals(), 0);
}

size_t	operator_count(t_input *data)
{
	size_t	hdoc_count;
	t_entry	*iter;

	iter = data->entries;
	hdoc_count = 0;
	while (iter != NULL)
	{
		if (iter->spec == HERE_DOC)
			hdoc_count++;
		iter = iter->next;
	}
	if (hdoc_count == 0)
		data->heredoc = NULL;
	return (hdoc_count);
}

int	here_doc(t_input *data)
{
	t_entry	*iter;
	size_t	hdoc_iter;
	size_t	hdoc_count;

	hdoc_iter = 0;
	iter = data->entries;
	hdoc_count = operator_count(data);
	if (hdoc_count == 0)
		return (0);
	data->heredoc = ft_calloc(hdoc_count + 1, sizeof(char *));
	if (!data->heredoc)
		return (-1);
	while (iter != NULL)
	{
		if (iter->spec == HERE_DOC)
		{
			if (hdoc_entry(iter, data, hdoc_iter) == -1)
				return (free2d(&data->heredoc), -1);
			hdoc_iter++;
		}
		iter = iter->next;
	}
	return (0);
}
