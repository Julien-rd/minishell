/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:05:54 by jromann           #+#    #+#             */
/*   Updated: 2025/09/16 18:05:10 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	var_len(char *buf, char **envp, t_input *data)
{
	size_t	iter;
	int		data_increase;

	iter = 0;
	while (buf[iter])
	{
		data_increase = 0;
		if (buf[iter] == '\'' && data->dbl_quote == 0)
			quoteclosed(&buf[iter], '\'', data);
		else if (buf[iter] == '\"' && data->sgl_quote == 0)
			quoteclosed(&buf[iter], '\"', data);
		else if (buf[iter] == '$' && data->sgl_quote == 0)
		{
			data->len += pathsize(&buf[iter + 1], envp, data);
			iter += pathlen(&buf[iter + 1]);
			data_increase = 1;
		}
		if (!data_increase)
			data->len++;
		iter++;
	}
}

static void	expand_str(char *buf, char **envp, t_input *data)
{
	size_t	iter;
	size_t	len;
	char	*sub_str;
	int		data_increase;

	iter = 0;
	len = 0;
	while (buf[iter])
	{
		data_increase = 0;
		if (buf[iter] == '\'' && data->dbl_quote == 0)
			quoteclosed(&buf[iter], '\'', data);
		else if (buf[iter] == '\"' && data->sgl_quote == 0)
			quoteclosed(&buf[iter], '\"', data);
		else if (buf[iter] == '$' && data->sgl_quote == 0)
		{
			sub_str = getpath(&buf[iter + 1], envp, data);
			ft_strlcpy(&data->expanded_str[len], sub_str, ft_strlen(sub_str) + 1);
            len += ft_strlen(sub_str);
			iter += pathlen(&buf[iter + 1]);
			data_increase = 1;
		}
		if (!data_increase)
		{
			ft_strlcpy(&data->expanded_str[len], &buf[iter], 2);
			len++;
		}
		iter++;
	}
}
void	expand_input(char *buf, char **envp, t_input *data)
{
	data->len = 0;
	data->dbl_quote = 0;
	data->sgl_quote = 0;
	var_len(buf, envp, data);
	data->expanded_str = calloc(sizeof(char), data->len);
	data->dbl_quote = 0;
	data->sgl_quote = 0;
	expand_str(buf, envp, data);
}