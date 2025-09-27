/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_input_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:22:55 by jromann           #+#    #+#             */
/*   Updated: 2025/09/27 11:56:46 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
size_t	envlen(char *env)
{
	size_t	len;

	len = 0;
	if (env[len] == '?')
		return (1);
	if (ft_isalpha(env[len]) || env[len] == '_')
		while (ft_isalnum(env[len]) || env[len] == '_')
			len++;
	return (len);
}

int	envcmp(const char *s1, const char *s2, size_t n)
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

int	get_env(char *buf, t_expanded_str *str, size_t env_iter, char **envp,
		size_t len)
{
	char	*var_value;
	size_t	iter;

	iter = 0;
	if (buf[0] == '?')
	{
		str->env_arr[env_iter] = ft_itoa(str->exit_code);
		if (!str->env_arr[env_iter])
			return (perror("get_env"), -1);
		return (1);
	}
	while (envp[iter])
	{
		if (envcmp(envp[iter], buf, len + 1))
		{
			str->env_arr[env_iter] = ft_substr(&envp[iter][len + 1], 0,
					ft_strlen(&envp[iter][len + 1]));
			if (!str->env_arr[env_iter])
				return (perror("get_env"), -1);
			return (1);
		}
		iter++;
	}
	return (0);
}
