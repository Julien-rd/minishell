/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_input_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:22:55 by jromann           #+#    #+#             */
/*   Updated: 2025/09/23 08:42:01 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	quoteclosed(char *str, char quote, t_input *data)
{
	size_t	iter;

	iter = 0;
	if (data->sgl_quote == 1)
	{
		data->sgl_quote = 0;
		return (1);
	}
	if (data->dbl_quote == 1)
	{
		data->dbl_quote = 0;
		return (0);
	}
	while (str[++iter])
	{
		if (str[iter] == quote)
		{
			if (quote == '\'')
			{
				data->sgl_quote = 1;
				return (1);
			}
			if (quote == '\"')
			{
				data->dbl_quote = 1;
				return (0);
			}
		}
	}
	return (0);
}
int	quote_check(size_t iter, char *buf, t_input *data)
{
	int	return_value;

	return_value = 0;
	if (buf[iter] == '\'' && data->dbl_quote == 0)
		return_value = quoteclosed(&buf[iter], '\'', data);
	else if (buf[iter] == '\"' && data->sgl_quote == 0)
		return_value = quoteclosed(&buf[iter], '\"', data);
	if (data->sgl_quote == 1)
		return (1);
	return (return_value);
}
size_t	pathlen(char *path)
{
	size_t	len;

	len = 0;
	if (path[len] == '?')
		return (1);
	if (ft_isalpha(path[len]) || path[len] == '_')
		while (ft_isalnum(path[len]) || path[len] == '_')
			len++;
	return (len);
}

int	pathcmp(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;

	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] == s2[i] && s1[i] != '\0' && i < n - 1)
	{
		i++;
	}
	if ((i == n - 1) && s1[i] == '=')
		return (1);
	return (0);
}

int	getpath(char *buf, t_expanded_str *str, size_t path_iter, char **envp,
		size_t len)
{
	char	*var_value;
	size_t	iter;

	iter = 0;
	if (buf[0] == '?')
	{
		str->paths[path_iter] = ft_itoa(str->exit_code);
		if (!str->paths[path_iter])
			return (perror("getpath"), -1);
		return (1);
	}
	while (envp[iter])
	{
		if (pathcmp(envp[iter], buf, len + 1))
		{
			str->paths[path_iter] = ft_substr(&envp[iter][len + 1], 0,
					ft_strlen(&envp[iter][len + 1]));
			if (!str->paths[path_iter])
				return (perror("getpath"), -1);
			return (1);
		}
		iter++;
	}
	return (0);
}
