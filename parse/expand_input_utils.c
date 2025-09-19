/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_input_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:22:55 by jromann           #+#    #+#             */
/*   Updated: 2025/09/18 20:14:11 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int quoteclosed(char *str, char quote, t_input *data)
{
	size_t iter;

	iter = 0;
	if (data->sgl_quote == 1)
	{
		data->sgl_quote = 0;
		return (1);
	}
	if (data->dbl_quote == 1)
	{
		data->dbl_quote = 0;
		return (1);
	}
	while (str[iter])
	{
		++iter;
		if (str[iter] && str[iter] == quote)
		{
			if (quote == '\'')
				data->sgl_quote = 1;
			if (quote == '\"')
				data->dbl_quote = 1;
			return (1);
		}
	}
	return (0);
}

size_t pathlen(char *path)
{
	size_t len;

	len = 0;
	if (ft_isdigit(path[len]))
		len = 1;
	if (ft_isalpha(path[len]) || path[len] == '_')
		while (ft_isalnum(path[len]) || path[len] == '_')
			len++;
	return (len);
}
size_t pathsize(char *path, char **envp, t_input *data)
{
	size_t len;
	size_t iter;
	char *str;

	iter = 0;
	len = pathlen(path);
	if (len == 0)
		return (1);
	while (envp[iter])
	{
		if (ft_strncmp(envp[iter], path, len + 1) == 61)
			return (ft_strlen(&envp[iter][len + 1]));
		iter++;
	}
	return (0);
}
char *getpath(char *path, char **envp, t_input *data)
{
	size_t len;
	size_t iter;
	char *str;

	len = 0;
	iter = 0;
	len = pathlen(path);
	if (len == 0)
		return (str = ft_calloc(2, 1), str = "$", str);
	// printf("[%s]\n", path);
	// fflush(stdout);
	while (envp[iter])
	{
		if (ft_strncmp(envp[iter], path, len + 1) == 61)
			return (ft_strdup(&envp[iter][len + 1]));
		iter++;
	}
	return (ft_calloc(1, 1));
}