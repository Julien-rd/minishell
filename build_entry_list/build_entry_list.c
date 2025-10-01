/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_entry_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 16:23:33 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/01 18:42:12 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	here_doc_caller(t_input *data)
{
	t_entry	*cur;
	
	cur = data->entries;
	while (cur)
	{
		if (cur->spec == HERE_DOC && here_doc(data) == -1)
		{
			if (g_current_signal == SIGINT)
				data->exit_code = 130;
			else
			{
				free2d(&data->envp);
				data->exit_code = 0;
			}
			return (-1);
		}
		cur = cur->next;
	}
	return (0);
}

static int	expand_raw_entry(t_input *data)
{
	t_entry	*cur;
	char	*expanded_str;
	
	cur = data->entries;
	while (cur)
	{
		if (!(cur->spec >= HERE_DOC_OP && cur->spec <= OUTFILE_OP)
			&& cur->spec != PIPE && cur->spec != HERE_DOC)
		{
			expanded_str = expand(cur, data);
			if (!expanded_str)
				return (-1);
			if (split_expands(expanded_str, cur, data) == -1)
				return (-1);
		}
		cur = cur->next;
	}
	return (0);
}

static void	entry_spec(t_input *data)
{
	t_entry	*cur;

	cur = data->entries;
	while (cur)
	{
		if (!ft_strcmp(cur->raw_entry, "<<"))
            cur->spec = HERE_DOC_OP;
        else if (!ft_strcmp(cur->raw_entry, ">>"))
            cur->spec = APPEND_OP;
        else if (!ft_strcmp(cur->raw_entry, "<"))
            cur->spec = INFILE_OP;
        else if (!ft_strcmp(cur->raw_entry, ">"))
            cur->spec = OUTFILE_OP;
        else if (!ft_strcmp(cur->raw_entry, "|"))
            cur->spec = PIPE;
        if (cur->next && cur->spec == HERE_DOC_OP)
            cur->next->spec = HERE_DOC;
        else if (cur->next && cur->spec == APPEND_OP)
            cur->next->spec = APPEND_FILE;
        else if (cur->next && cur->spec == INFILE_OP)
            cur->next->spec = INFILE;
        else if (cur->next && cur->spec == OUTFILE_OP)
            cur->next->spec = OUTFILE;
		cur = cur->next;
	}
}

static int	create_list(char *buf, t_input *data)
{
	size_t	iter;
	size_t	entry_len;
	t_entry	*entry;
	char	*raw_str;

	data->entries = NULL;
	iter = 0;
	while (buf[iter])
	{
		iter += skip_whitspaces(&buf[iter]);
		if (!buf[iter])
			break;
		entry_len = token_len(buf, data, iter);
		raw_str = malloc((entry_len + 1) * sizeof(char));
		if (!raw_str)
			return (perror("create_list"), -1);
		ft_strlcpy(raw_str, &buf[iter], entry_len + 1);
		entry = newnode(raw_str);
		if (!entry)
			return (perror("create_list"), free(raw_str), -1);
		lstadd(&data->entries, entry);
		iter += entry_len;
	}
	return (0);
}

int	build_entry_list(char *buf, t_input *data)
{
	if (create_list(buf, data) == -1)
		return (-1);
	entry_spec(data);
	if (expand_raw_entry(data) == -1)
		return (-1);
	if (here_doc_caller(data) == -1)
		return (-1);
	return (0);
}