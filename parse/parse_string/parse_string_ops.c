/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_string_ops.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:53:08 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/24 14:14:18 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	op_count(char *buf, size_t *iter, size_t *count)
{
	if (!ft_strncmp(&buf[*iter], ">>", 2) || !ft_strncmp(&buf[*iter], "<<", 2))
	{
		(*count)++;
		(*iter)++;
	}
	else if (buf[*iter] == '>' || buf[*iter] == '<' || buf[*iter] == '|')
		(*count)++;
	(*iter)++;
}

void	fill_ops(size_t *entry, size_t *iter, t_input *data)
{
	while (data->exp_str[*iter] && !is_token(data->exp_str[*iter]))
	{
		if (data->exp_str[*iter] == '|')
			ft_strlcpy(data->entries[(*entry)++], "|", 2);
		if (!ft_strncmp(&data->exp_str[*iter], ">>", 2))
		{
			ft_strlcpy(data->entries[(*entry)++], ">>", 3);
			(*iter)++;
		}
		else if (data->exp_str[*iter] == '>')
			ft_strlcpy(data->entries[(*entry)++], ">", 2);
		else if (!ft_strncmp(&data->exp_str[*iter], "<<", 2))
		{
			ft_strlcpy(data->entries[(*entry)++], "<<", 3);
			(*iter)++;
		}
		else if (data->exp_str[*iter] == '<')
			ft_strlcpy(data->entries[(*entry)++], "<", 2);
		(*iter)++;
	}
}

int	malloc_ops(size_t *entry, size_t *iter, t_input *data)
{
	while (data->exp_str[*iter] && !is_token(data->exp_str[*iter]))
	{
		if (!ft_strncmp(&data->exp_str[*iter], "<<", 2)
			|| !ft_strncmp(&data->exp_str[*iter], ">>", 2))
		{
			data->input_spec[*entry] = OPERATOR;
			data->entries[*entry] = malloc(3 * sizeof(char));
			if (!data->entries[*entry])
				return (-1);
			(*entry)++;
			(*iter)++;
		}
		else if (data->exp_str[*iter] == '|' || data->exp_str[*iter] == '>'
			|| data->exp_str[*iter] == '<')
		{
			data->input_spec[*entry] = OPERATOR;
			data->entries[*entry] = malloc(2 * sizeof(char));
			if (!data->entries[*entry])
				return (-1);
			(*entry)++;
		}
		(*iter)++;
	}
	return (0);
}
