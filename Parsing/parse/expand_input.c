/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:05:54 by jromann           #+#    #+#             */
/*   Updated: 2025/09/23 10:31:50 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	expanded_str(char *buf, t_input *data, t_expanded_str *str)
{
	size_t	iter;
	size_t	str_iter;
	size_t	len;
	size_t	path_iter;
	size_t	path_pos_iter;

	path_iter = 0;
	path_pos_iter = 0;
	str_iter = 0;
	iter = 0;
	data->exp_str = calloc(data->len + 1, sizeof(char));
	if (!data->exp_str)
		return (-1);
	while (buf[iter])
	{
		if (str->var_count * 2 > path_pos_iter && str->path_pos[path_pos_iter] == iter)
		{
			if (str->path_pos[path_pos_iter + 1] == 0)
			{
				iter += pathlen(&buf[iter + 1]);
				path_pos_iter += 2;
			}
			else
			{
				len = ft_strlen(str->paths[path_iter]);
				ft_memcpy(&data->exp_str[str_iter], str->paths[path_iter],
					len);
				path_iter++;
				str_iter += len;
				path_pos_iter += 2;
				iter += pathlen(&buf[iter + 1]);
			}
		}
		else
			data->exp_str[str_iter++] = buf[iter];
		iter++;
	}
	return (0);
}

int	check_paths(char *buf, t_input *data, t_expanded_str *str, char **envp)
{
	size_t	iter;
	size_t	paths_iter;
	size_t	path_pos_iter;
	size_t	len;
	size_t	path_return;

	paths_iter = 0;
	path_pos_iter = 0;
	iter = 0;
	data->dbl_quote = 0;
	data->sgl_quote = 0;
	while (buf[iter])
	{
		len = pathlen(&buf[iter + 1]);
		if (!quote_check(iter, buf, data) && buf[iter] == '$' && len)
		{
			path_return = getpath(&buf[iter + 1], str, paths_iter, envp, len);
			if (path_return == 1)
			{
				data->len += ft_strlen(str->paths[paths_iter]);
				paths_iter++;
				str->path_pos[path_pos_iter++] = iter;
				str->path_pos[path_pos_iter++] = iter + len;
			}
			else if (path_return == 0)
			{
				str->path_pos[path_pos_iter++] = iter;
				str->path_pos[path_pos_iter++] = 0;
			}
			else
				return (-1);
			iter += len;
		}
		else
			data->len++;
		iter++;
	}
	return (0);
}

int	paths_init(char *buf, t_input *data, t_expanded_str *str)
{
	size_t	iter;

	iter = 0;
	data->dbl_quote = 0;
	data->sgl_quote = 0;
	while (buf[iter])
	{
		if (!quote_check(iter, buf, data) && buf[iter] == '$')
		{
			if (pathlen(&buf[iter + 1]) > 0)
				str->var_count++;
		}
		iter++;
	}
	if (str->var_count > 0)
	{
		str->paths = calloc(sizeof(char *), str->var_count + 1);
		if (!str->paths)
			return (perror(""), -1);
		str->path_pos = malloc(sizeof(size_t) * str->var_count * 2);
		if (!str->path_pos)
			return (free2d(str->paths), perror(""), -1);
	}
	return (0);
}

int	expand_input(char *buf, char **envp, t_input *data)
{
	char			**paths;
	t_expanded_str	str;
	size_t			iter;

	data->len = 0;
	data->exp_str_malloc = 1;
	str.var_count = 0;
	str.exit_code = data->exit_code;
	str.paths = NULL;
	if (paths_init(buf, data, &str) == -1)
		return (-1);
	if (str.var_count == 0)
	{
		data->exp_str = buf;
		data->exp_str_malloc = 0;
		return (0);
	}
	if (check_paths(buf, data, &str, envp) == -1)
		return (free2d(str.paths), free(str.path_pos), -1);
	if (expanded_str(buf, data, &str) == -1)
		return (free2d(str.paths), free(str.path_pos), -1);
	free2d(str.paths);
	free(str.path_pos);
	return (0);
}
