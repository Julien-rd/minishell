/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:44:43 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/16 12:58:27 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_token(char c)
{
	if (c == 32 || (c >= 9 && c <=13) || c == '>' || c == '<' || c == '|')
		return (0);
	return (1);
}

int	is_closed(char *str)
{
	size_t	iter;
	int		tmp_sgl_quote;

	iter = 0;
	tmp_sgl_quote = 0;
	while (str[iter])
	{
		if (str[iter] == '\'' && ft_strchr(&str[iter + 1], '\''))
			tmp_sgl_quote = 1;
		else if (str[iter] == '\'' && tmp_sgl_quote == 1)
			tmp_sgl_quote = 0;
		if (str[iter] == '\"' && tmp_sgl_quote == 0)
			return (1);
		iter++;
	}
	return (0);
}

size_t	count_entries(char *buf, t_input *data)
{
	size_t	iter;
	size_t	count;

	iter = 0;
	count = 0;
	data->dbl_quote = 0;
	data->sgl_quote = 0;
	while (buf[iter] && !is_token(buf[iter]))
	{
		if ((buf[iter] == '>' && buf[iter + 1] == '>') || (buf[iter] == '<' && buf[iter + 1] == '<'))
		{
			count++;
			iter++;
		}
		else if (buf[iter] == '>' || buf[iter] == '<' || buf[iter] == '|')
			count++;
		iter++;
	}
	while (buf[iter])
	{
		count++;
		while (data->dbl_quote || data->sgl_quote || (buf[iter] && is_token(buf[iter])))
		{
			if (data->dbl_quote == 0 && data->sgl_quote == 0 && buf[iter] == '\'' && ft_strchr(&buf[iter + 1], '\''))
				data->sgl_quote = 1;
			else if (data->dbl_quote == 0 && data->sgl_quote == 1 && buf[iter] == '\'')
				data->sgl_quote = 0;
			if (data->sgl_quote == 0 && data->dbl_quote == 0 && buf[iter] == '\"' && ft_strchr(&buf[iter + 1], '\"'))
				data->dbl_quote = 1;
			else if (data->sgl_quote == 0 && data->dbl_quote == 1 && buf[iter] == '\"')
				data->dbl_quote = 0;
			iter++;
		}
		while (buf[iter] && !is_token(buf[iter]))
		{
			if ((buf[iter] == '>' && buf[iter + 1] == '>') || (buf[iter] == '<' && buf[iter + 1] == '<'))
			{
				count++;
				iter++;
			}
			else if (buf[iter] == '>' || buf[iter] == '<' || buf[iter] == '|')
				count++;	
			iter++;
		}
	}
	return (count);
}

int	malloc_ops(size_t *entry, size_t *iter, char *buf, t_input *data)
{
	if (buf[0] == '|')
	{
		data->input_spec[*entry] = OPERATOR;
		data->entries[(*entry)++] = malloc(2 * sizeof(char));
	}
	else if (buf[0] == '>' && buf[1] == '>')
	{
		data->input_spec[*entry] = OPERATOR;
		data->entries[(*entry)++] = malloc(3 * sizeof(char));
		(*iter)++;
	}
	else if (buf[0] == '>')
	{
		data->input_spec[*entry] = OPERATOR;
		data->entries[(*entry)++] = malloc(2 * sizeof(char));
	}
	else if (buf[0] == '<' && buf[1] == '<')
	{
		data->input_spec[*entry] = OPERATOR;
		data->entries[(*entry)++] = malloc(3 * sizeof(char));
		(*iter)++;
	}
	else if (buf[0] == '<')
	{
		data->input_spec[*entry] = OPERATOR;
		data->entries[(*entry)++] = malloc(2 * sizeof(char));
	}
	return (0);
}

void fill_ops(size_t *entry, size_t *iter, char *buf, t_input *data)
{
	if (buf[0] == '|')
	{
		data->entries[*entry][0] = '|';
		data->entries[*entry][1] = '\0';
		(*entry)++;
	}
	else if (buf[0] == '>' && buf[1] == '>')
	{
		data->entries[*entry][0] = '>';
		data->entries[*entry][1] = '>';
		data->entries[*entry][2] = '\0';
		(*entry)++;
		(*iter)++;
	}
	else if (buf[0] == '>')
	{
		data->entries[*entry][0] = '>';
		data->entries[*entry][1] = '\0';
		(*entry)++;
	}
	else if (buf[0] == '<' && buf[1] == '<')
	{
		data->entries[*entry][0] = '<';
		data->entries[*entry][1] = '<';
		data->entries[*entry][2] = '\0';
		(*entry)++;
		(*iter)++;
	}
	else if (buf[0] == '<')
	{
		data->entries[*entry][0] = '<';
		data->entries[*entry][1] = '\0';
		(*entry)++;
	}
}

int	malloc_entries(char *buf, t_input *data)
{
	size_t	iter;
	size_t	tmp_count;
	size_t	entry;

	iter = 0;
	entry = 0;
	while (buf[iter] && !is_token(buf[iter]))
	{
		if (malloc_ops(&entry, &iter, &buf[iter], data) == -1)
			return (-1);
		iter++;
	}
	while (buf[iter])
	{
		tmp_count = 0;
		while (data->dbl_quote || data->sgl_quote || (buf[iter] && is_token(buf[iter])))
		{
			if (data->dbl_quote == 0 && data->sgl_quote == 0 && buf[iter] == '\'' && ft_strchr(&buf[iter + 1], '\''))
				data->sgl_quote = 1, iter++;
			if (data->sgl_quote == 0 && data->dbl_quote == 0 && buf[iter] == '\"' && ft_strchr(&buf[iter + 1], '\"'))
				data->dbl_quote = 1, iter++;
			if (!(data->sgl_quote == 1 && buf[iter] == '\'') && !(data->dbl_quote == 1 && buf[iter] == '\"'))
				tmp_count++;
			if (data->dbl_quote == 0 && data->sgl_quote == 1 && buf[iter] == '\'')
				data->sgl_quote = 0;
			if (data->sgl_quote == 0 && data->dbl_quote == 1 && buf[iter] == '\"')
				data->dbl_quote = 0;
			iter++;
		}
		data->entries[entry] = malloc((tmp_count + 1) * sizeof(char));
		entry++;
		while (buf[iter] && !is_token(buf[iter]))
		{
			if (malloc_ops(&entry, &iter, &buf[iter], data) == -1)
				return (-1);
			iter++;
		}
	}
	data->entries[entry] = NULL;
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
}

void	fill_entries(char *buf, t_input *data)
{
	size_t	iter;
	size_t	tmp_count;
	size_t	entry;

	iter = 0;
	entry = 0;
	while (buf[iter] && !is_token(buf[iter]))
	{
		fill_ops(&entry, &iter, &buf[iter], data);
		iter++;
	}
	while (buf[iter])
	{
		tmp_count = 0;
		while (data->dbl_quote || data->sgl_quote || (buf[iter] && is_token(buf[iter])))
		{
			if (data->dbl_quote == 0 && data->sgl_quote == 0 && buf[iter] == '\'' && ft_strchr(&buf[iter + 1], '\''))
				data->sgl_quote = 1, iter++;
			if (data->sgl_quote == 0 && data->dbl_quote == 0 && buf[iter] == '\"' && ft_strchr(&buf[iter + 1], '\"'))
				data->dbl_quote = 1, iter++;
			if (!(data->sgl_quote == 1 && buf[iter] == '\'') && !(data->dbl_quote == 1 && buf[iter] == '\"'))
				data->entries[entry][tmp_count++] = buf[iter];
			if (data->dbl_quote == 0 && data->sgl_quote == 1 && buf[iter] == '\'')
				data->sgl_quote = 0;
			if (data->sgl_quote == 0 && data->dbl_quote == 1 && buf[iter] == '\"')
				data->dbl_quote = 0;
			iter++;
		}
		data->entries[entry][tmp_count] = '\0';
		entry++;
		while (buf[iter] && !is_token(buf[iter]))
		{
			fill_ops(&entry, &iter, &buf[iter], data);
			iter++;
		}
	}
}

int	parse_string(char *buf, t_input *data)
{
	data->total_entries = count_entries(buf, data);
	data->entries = malloc(sizeof(char *) * (data->total_entries + 1));
	data->input_spec = malloc(sizeof(int) * data->total_entries);
	input_spec_init(data);
	if (!data->entries)
		return (-1);
	if (malloc_entries(buf, data) == -1)
		return (-1);
	fill_entries(buf, data);

	size_t	 iter = 0;
	while (data->entries[iter])
	{
		printf("%zu = [%s] (%i)\n", iter, data->entries[iter], data->input_spec[iter]);
		iter++;
	}
	return (0);
}