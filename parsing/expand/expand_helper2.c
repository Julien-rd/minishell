/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:22:55 by jromann           #+#    #+#             */
/*   Updated: 2025/10/03 15:02:11 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	quoteclosed(char *str, char quote, t_sh *sh)
{
	size_t	iter;

	iter = 0;
	if (sh->sgl_quote == 1)
	{
		sh->sgl_quote = 0;
		return (1);
	}
	if (sh->dbl_quote == 1)
	{
		sh->dbl_quote = 0;
		return (0);
	}
	while (str[++iter])
	{
		if (str[iter] == quote)
		{
			sh->sgl_quote = (quote == '\'');
			sh->dbl_quote = (quote == '\"');
			return (sh->sgl_quote);
		}
	}
	return (0);
}

int	quote_check(size_t iter, char *buf, t_sh *sh)
{
	int	return_value;

	return_value = 0;
	if (buf[iter] == '\'' && sh->dbl_quote == 0)
		return_value = quoteclosed(&buf[iter], '\'', sh);
	else if (buf[iter] == '\"' && sh->sgl_quote == 0)
		return_value = quoteclosed(&buf[iter], '\"', sh);
	if (sh->sgl_quote == 1)
		return (1);
	return (0);
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

int	get_env(char *buf, t_expand_str *str, t_expand_helper *exh, char **envp)
{
	size_t	iter;

	iter = 0;
	if (buf[0] == '?')
	{
		if (g_current_signal == SIGINT)
			str->exit_code = 130;
		str->env_arr[exh->env_iter] = ft_itoa(str->exit_code);
		if (!str->env_arr[exh->env_iter])
			return (perror("get_env"), -1);
		return (1);
	}
	while (envp[iter])
	{
		if (envcmp(envp[iter], buf, exh->len + 1))
		{
			str->env_arr[exh->env_iter] = ft_substr(&envp[iter][exh->len + 1],
					0, ft_strlen(&envp[iter][exh->len + 1]));
			if (!str->env_arr[exh->env_iter])
				return (perror("get_env"), -1);
			return (1);
		}
		iter++;
	}
	return (0);
}
