/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:44:43 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/29 13:33:16 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	count_entries(char *buf, t_input *data)
{
	size_t	iter;
	size_t	count;

	iter = 0;
	count = 0;
	data->dbl_quote = 0;
	data->sgl_quote = 0;
	while (buf[iter] && !is_token(buf[iter]))
		op_count(buf, &iter, &count);
	while (buf[iter])
	{
		count++;
		while (data->dbl_quote || data->sgl_quote || (buf[iter]
				&& is_token(buf[iter])))
		{
			toggle_quotes(buf, data, iter);
			iter++;
		}
		while (buf[iter] && !is_token(buf[iter]))
			op_count(buf, &iter, &count);
	}
	return (count);
}

void	input_spec_init(t_input *data)
{
	size_t	iter;

	iter = 0;
	while (iter < data->total_entries)
	{
		data->input_spec[iter] = DEFAULT;
		iter++;
	}
	data->input_spec[iter] = END;
}

int	malloc_entries(char *buf, t_input *data)
{
	size_t	iter;
	size_t	tmp_count;
	size_t	entry;

	iter = 0;
	entry = 0;
	if (malloc_ops(buf, &entry, &iter, data) == -1)
		return (-1);
	while (buf[iter])
	{
		tmp_count = token_len(buf, data, &iter);
		data->entries[entry] = malloc((tmp_count + 1) * sizeof(char));
		if (!data->entries[entry])
			return (-1);
		entry++;
		if (malloc_ops(buf, &entry, &iter, data) == -1)
			return (-1);
	}
	data->entries[entry] = NULL;
	return (0);
}

void	fill_entries(char *buf, t_input *data)
{
	size_t	iter;
	size_t	tmp_count;
	size_t	entry;

	iter = 0;
	entry = 0;
	fill_ops(buf, &entry, &iter, data);
	while (buf[iter])
	{
		tmp_count = 0;
		while (data->dbl_quote || data->sgl_quote || (buf[iter]
				&& is_token(buf[iter])))
		{
			toggle_quotes(buf, data, iter);
			data->entries[entry][tmp_count++] = buf[iter];
			iter++;
		}
		data->entries[entry][tmp_count] = '\0';
		entry++;
		fill_ops(buf, &entry, &iter, data);
	}
}

int	syntax_check(t_input *data)
{
	size_t	iter;

	iter = 0;
	while (data->input_spec[iter])
	{
		if (data->input_spec[iter] == PIPE)
		{
			if (data->input_spec[iter + 1] == END || data->input_spec[iter
				+ 1] == PIPE || iter == 0 || (iter != 0 && data->input_spec[iter
					- 1] == PIPE))
			{
				write(1, "syntax error near unexpected token `|'\n", 40);
				return (-1);
			}
		}
		if (data->input_spec[iter] == INFILE_OP && data->input_spec[iter
			+ 1] != INFILE)
		{
			write(1, "syntax error near unexpected token `<'\n", 40);
			return (-1);
		}
		if (data->input_spec[iter] == HERE_DOC_OP && data->input_spec[iter
			+ 1] != HERE_DOC)
		{
			write(1, "syntax error near unexpected token `<<'\n", 41);
			return (-1);
		}
		if (data->input_spec[iter] == OUTFILE_OP && data->input_spec[iter
			+ 1] != OUTFILE)
		{
			write(1, "syntax error near unexpected token `>'\n", 40);
			return (-1);
		}
		if (data->input_spec[iter] == APPEND_OP && data->input_spec[iter
			+ 1] != APPEND_FILE)
		{
			write(1, "syntax error near unexpected token `>>'\n", 41);
			return (-1);
		}
		iter++;
	}
	return (0);
}

int	parse_string(char *buf, t_input *data)
{
	data->total_entries = count_entries(buf, data);
	data->entries = malloc(sizeof(char *) * (data->total_entries + 1));
	if (!data->entries)
		return (perror("parsing"), -1);
	data->input_spec = malloc(sizeof(int) * (data->total_entries + 1));
	if (!data->input_spec)
		return (perror("parsing"), free(data->entries), -1);
	input_spec_init(data);
	if (malloc_entries(buf, data) == -1)
		return (perror("parsing"), free2d(&data->entries),
			free(data->input_spec), -1);
	fill_entries(buf, data);
	entry_spec(data);
	return (0);
}
