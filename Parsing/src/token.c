/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:05:54 by jromann           #+#    #+#             */
/*   Updated: 2025/09/16 16:46:37 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	token_len(char *buf, char **envp, t_input *data)
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

static void	token_str(char *buf, char **envp, t_input *data)
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
			ft_strlcpy(&data->token_str[len], sub_str, ft_strlen(sub_str) + 1);
            len += ft_strlen(sub_str);
			iter += pathlen(&buf[iter + 1]);
			data_increase = 1;
		}
		if (!data_increase)
		{
			ft_strlcpy(&data->token_str[len], &buf[iter], 2);
			len++;
		}
		iter++;
	}
}
void	token(char *buf, char **envp, t_input *data)
{
	data->len = 0;
	data->dbl_quote = 0;
	data->sgl_quote = 0;
	token_len(buf, envp, data);
	data->token_str = calloc(sizeof(char), data->len);
	data->dbl_quote = 0;
	data->sgl_quote = 0;
	token_str(buf, envp, data);
}