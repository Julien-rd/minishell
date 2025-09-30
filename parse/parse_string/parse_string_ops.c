/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_string_ops.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:53:08 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/30 12:10:56 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// void	fill_ops(char *buf, size_t *entry, size_t *iter, t_input *data)
// {
// 	while (buf[*iter] && !is_token(buf[*iter]))
// 	{
// 		if (buf[*iter] == '|')
// 			ft_strlcpy(data->entries[(*entry)++], "|", 2);
// 		else if (!ft_strncmp(&buf[*iter], ">>", 2))
// 		{
// 			ft_strlcpy(data->entries[(*entry)++], ">>", 3);
// 			(*iter)++;
// 		}
// 		else if (buf[*iter] == '>')
// 			ft_strlcpy(data->entries[(*entry)++], ">", 2);
// 		else if (!ft_strncmp(&buf[*iter], "<<", 2))
// 		{
// 			ft_strlcpy(data->entries[(*entry)++], "<<", 3);
// 			(*iter)++;
// 		}
// 		else if (buf[*iter] == '<')
// 			ft_strlcpy(data->entries[(*entry)++], "<", 2);
// 		(*iter)++;
// 	}
// }

// int	malloc_ops(char *buf, size_t *entry, size_t *iter, t_input *data)
// {
// 	while (buf[*iter] && !is_token(buf[*iter]))
// 	{
// 		if (!ft_strncmp(&buf[*iter], "<<", 2)
// 			|| !ft_strncmp(&buf[*iter], ">>", 2))
// 		{
// 			data->input_spec[*entry] = OPERATOR;
// 			data->entries[*entry] = malloc(3 * sizeof(char));
// 			if (!data->entries[*entry])
// 				return (-1);
// 			(*entry)++;
// 			(*iter)++;
// 		}
// 		else if (buf[*iter] == '|' || buf[*iter] == '>'
// 			|| buf[*iter] == '<')
// 		{
// 			data->input_spec[*entry] = OPERATOR;
// 			data->entries[*entry] = malloc(2 * sizeof(char));
// 			if (!data->entries[*entry])
// 				return (-1);
// 			(*entry)++;
// 		}
// 		(*iter)++;
// 	}
// 	return (0);
// }
