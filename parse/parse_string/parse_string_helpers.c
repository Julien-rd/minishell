/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_string_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:46:22 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/29 13:27:59 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_token(char c)
{
	if (c == 32 || (c >= 9 && c <= 13) || c == '>' || c == '<' || c == '|')
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

void	toggle_quotes(char *buf, t_input *data, size_t iter)
{
	if (buf[iter] == '\'' && !data->dbl_quote && (data->sgl_quote
			|| (ft_strchr(&buf[iter + 1], '\''))))
		data->sgl_quote = !data->sgl_quote;
	else if (buf[iter] == '\"' && !data->sgl_quote && (data->dbl_quote
			|| (ft_strchr(&buf[iter + 1], '\"'))))
		data->dbl_quote = !data->dbl_quote;
}

int	token_len(char *buf, t_input *data, size_t *iter)
{
	size_t	count;

	count = 0;
	while (data->dbl_quote || data->sgl_quote
		|| (buf[*iter] && is_token(buf[*iter])))
	{
		toggle_quotes(buf, data, *iter);
		count++;
		(*iter)++;
	}
	return (count);
}
