/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:44:43 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/30 18:51:18 by jromann          ###   ########.fr       */
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

// int	malloc_entries(char *buf, t_input *data)
// {
// 	size_t	iter;
// 	size_t	tmp_count;
// 	size_t	entry;

// 	iter = 0;
// 	entry = 0;
// 	if (malloc_ops(buf, &entry, &iter, data) == -1)
// 		return (-1);
// 	while (buf[iter])
// 	{
// 		tmp_count = token_len(buf, data, &iter);
// 		data->entries[entry] = malloc((tmp_count + 1) * sizeof(char));
// 		if (!data->entries[entry])
// 			return (-1);
// 		entry++;
// 		if (malloc_ops(buf, &entry, &iter, data) == -1)
// 			return (-1);
// 	}
// 	data->entries[entry] = NULL;
// 	return (0);
// }

// void	fill_entries(char *buf, t_input *data)
// {
// 	size_t	iter;
// 	size_t	tmp_count;
// 	size_t	entry;

// 	iter = 0;
// 	entry = 0;
// 	fill_ops(buf, &entry, &iter, data);
// 	while (buf[iter])
// 	{
// 		tmp_count = 0;
// 		while (data->dbl_quote || data->sgl_quote || (buf[iter]
// 				&& is_token(buf[iter])))
// 		{
// 			toggle_quotes(buf, data, iter);
// 			data->entries[entry][tmp_count++] = buf[iter];
// 			iter++;
// 		}
// 		data->entries[entry][tmp_count] = '\0';
// 		entry++;
// 		fill_ops(buf, &entry, &iter, data);
// 	}
// }

int	syntax_check(t_input *data)
{
	size_t	iter;
	t_entry	*entry;

	entry = data->entries;
	iter = 0;
	while (entry != NULL)
	{
		if (entry->spec == PIPE)
		{
			if (entry->next == NULL || entry->next->spec == PIPE
				|| entry == data->entries)
			{
				write(1, "syntax error near unexpected token `|'\n", 40);
				return (-1);
			}
		}
		if (entry->spec == INFILE_OP && (entry->next == NULL
				|| entry->next->spec != INFILE))
		{
			if (entry->next == NULL)
				write(1, "syntax error near unexpected token `newline'\n", 45);
			else
			{
				write(1, "syntax error near unexpected token `", 36);
				write(1, entry->next->raw_entry,
					ft_strlen(entry->next->raw_entry));
				write(1, "'\n", 2);
			}
			return (-1);
		}
		if (entry->spec == HERE_DOC_OP && (entry->next == NULL
				|| entry->next->spec != HERE_DOC))
		{
			if (entry->next == NULL)
				write(1, "syntax error near unexpected token `newline'\n", 45);
			else
			{
				write(1, "syntax error near unexpected token `", 36);
				write(1, entry->next->raw_entry,
					ft_strlen(entry->next->raw_entry));
				write(1, "'\n", 2);
			}
			return (-1);
		}
		if (entry->spec == OUTFILE_OP && (entry->next == NULL
				|| entry->next->spec != OUTFILE))
		{
			if (entry->next == NULL)
				write(1, "syntax error near unexpected token `newline'\n", 45);
			else
			{
				write(1, "syntax error near unexpected token `", 36);
				write(1, entry->next->raw_entry,
					ft_strlen(entry->next->raw_entry));
				write(1, "'\n", 2);
			}
			return (-1);
		}
		if (entry->spec == APPEND_OP && (entry->next == NULL
				|| entry->next->spec != APPEND_FILE))
		{
			if (entry->next == NULL)
				write(1, "syntax error near unexpected token `newline'\n", 45);
			else
			{
				write(1, "syntax error near unexpected token `", 36);
				write(1, entry->next->raw_entry,
					ft_strlen(entry->next->raw_entry));
				write(1, "'\n", 2);
			}
			return (-1);
		}
		entry = entry->next;
	}
	return (0);
}

// int	parse_string(char *buf, t_input *data)
// {
// 	data->total_entries = count_entries(buf, data);
// 	data->entries = malloc(sizeof(char *) * (data->total_entries + 1));
// 	if (!data->entries)
// 		return (perror("parsing"), -1);
// 	data->input_spec = malloc(sizeof(int) * (data->total_entries + 1));
// 	if (!data->input_spec)
// 		return (perror("parsing"), free(data->entries), -1);
// 	input_spec_init(data);
// 	if (malloc_entries(buf, data) == -1)
// 		return (perror("parsing"), free2d(&data->entries),
// 			free(data->input_spec), -1);
// 	fill_entries(buf, data);
// 	entry_spec(data);
// 	return (0);
// }
