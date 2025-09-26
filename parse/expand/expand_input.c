/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:05:54 by jromann           #+#    #+#             */
/*   Updated: 2025/09/26 13:02:11 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	expanded_str(char *buf, t_input *data, t_expanded_str *str)
{
	size_t	iter;
	size_t	str_iter;
	size_t	len;
	size_t	env_iter;
	size_t	env_pos_iter;

	env_iter = 0;
	env_pos_iter = 0;
	str_iter = 0;
	iter = 0;
	data->exp_str = calloc(data->len + 1, sizeof(char));
	if (!data->exp_str)
		return (-1);
	while (buf[iter])
	{
		if (str->var_count * 2 > env_pos_iter && str->env_pos[env_pos_iter] == iter)
		{
			if (str->env_pos[env_pos_iter + 1] == 0)
			{
				iter += envlen(&buf[iter + 1]);
				env_pos_iter += 2;
			}
			else
			{
				len = ft_strlen(str->env_arr[env_iter]);
				ft_memcpy(&data->exp_str[str_iter], str->env_arr[env_iter],
					len);
				env_iter++;
				str_iter += len;
				env_pos_iter += 2;
				iter += envlen(&buf[iter + 1]);
			}
		}
		else
			data->exp_str[str_iter++] = buf[iter];
		iter++;
	}
	return (0);
}

int	check_envs(char *buf, t_input *data, t_expanded_str *str, char **envp)
{
	size_t	iter;
	size_t	env_iter;
	size_t	env_pos_iter;
	size_t	len;
	size_t	env_return;

	env_iter = 0;
	env_pos_iter = 0;
	iter = 0;
	data->dbl_quote = 0;
	data->sgl_quote = 0;
	while (buf[iter])
	{
		len = envlen(&buf[iter + 1]);
		if (!quote_check(iter, buf, data) && buf[iter] == '$' && len)
		{
			env_return = get_env(&buf[iter + 1], str, env_iter, envp, len);
			if (env_return == 1)
			{
				data->len += ft_strlen(str->env_arr[env_iter]);
				env_iter++;
				str->env_pos[env_pos_iter++] = iter;
				str->env_pos[env_pos_iter++] = iter + len;
			}
			else if (env_return == 0)
			{
				str->env_pos[env_pos_iter++] = iter;
				str->env_pos[env_pos_iter++] = 0;
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

int	env_init(char *buf, t_input *data, t_expanded_str *str)
{
	size_t	iter;

	iter = 0;
	data->dbl_quote = 0;
	data->sgl_quote = 0;
	while (buf[iter])
	{
		if (!quote_check(iter, buf, data) && buf[iter] == '$')
		{
			if (envlen(&buf[iter + 1]) > 0)
				str->var_count++;
		}
		iter++;
	}
	if (str->var_count > 0)
	{
		str->env_arr = calloc(sizeof(char *), str->var_count + 1);
		if (!str->env_arr)
			return (perror(""), -1);
		str->env_pos = malloc(sizeof(size_t) * str->var_count * 2);
		if (!str->env_pos)
			return (free(str->env_arr), perror(""), -1);
	}
	return (0);
}

int	expand(char *buf, char **envp, t_input *data)
{
	char			**env_arr;
	t_expanded_str	str;
	size_t			iter;

	data->len = 0;
	data->exp_str_malloc = 1;
	str.var_count = 0;
	str.exit_code = data->exit_code;
	str.env_arr = NULL;
	if (env_init(buf, data, &str) == -1)
		return (-1);
	if (str.var_count == 0)
	{
		data->exp_str = buf;
		data->exp_str_malloc = 0;
		return (0);
	}
	if (check_envs(buf, data, &str, envp) == -1)
		return (free2d(&str.env_arr), free(str.env_pos), -1);
	if (expanded_str(buf, data, &str) == -1)
		return (free2d(&str.env_arr), free(str.env_pos), -1);
	free2d(&str.env_arr);
	free(str.env_pos);
	return (0);
}
