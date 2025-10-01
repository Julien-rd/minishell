/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 14:58:44 by jromann           #+#    #+#             */
/*   Updated: 2025/10/01 20:53:10 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ex_encounter(char *str_new, t_expand_helper *exh,
		t_expand_str *str, size_t iter)
{
	if (str->env_pos[exh->env_pos_iter + 1] == 0)
		exh->env_pos_iter += 2;
	else
	{
		exh->len = ft_strlen(str->env_arr[exh->env_iter]);
		ft_memcpy(str_new, str->env_arr[exh->env_iter], exh->len);
		exh->env_iter++;
		exh->str_iter += exh->len;
		exh->env_pos_iter += 2;
	}
	return (envlen(&exh->buf[iter + 1]));
}

static int	check_return_get_env(size_t iter, t_expand_str *str,
		t_expand_helper *exh, t_input *data)
{
	if (exh->env_return == 1)
	{
		data->len += ft_strlen(str->env_arr[exh->env_iter]);
		exh->env_iter++;
		str->env_pos[exh->env_pos_iter++] = iter;
		str->env_pos[exh->env_pos_iter++] = iter + exh->len;
	}
	else if (exh->env_return == 0)
	{
		str->env_pos[exh->env_pos_iter++] = iter;
		str->env_pos[exh->env_pos_iter++] = 0;
	}
	else
		return (-1);
	return (0);
}

char	*expanded_str(char *buf, t_input *data, t_expand_str *str)
{
	size_t			iter;
	t_expand_helper	exh;
	char			*exp_str;

	exh.env_iter = 0;
	exh.env_pos_iter = 0;
	exh.str_iter = 0;
	iter = 0;
	data->dbl_quote = 0;
	data->sgl_quote = 0;
	exh.buf = buf;
	exp_str = ft_calloc(sizeof(char), data->len + 1);
	while (exp_str && buf[iter])
	{
		toggle_quotes(buf, data, iter);
		if (str->var_count * 2 > exh.env_pos_iter
			&& str->env_pos[exh.env_pos_iter] == iter && (data->sgl_quote == 0 || str->flag == HERE_DOC))
			iter += ex_encounter(&exp_str[exh.str_iter], &exh, str, iter);
		else
			exp_str[exh.str_iter++] = buf[iter];
		toggle_quotes(buf, data, iter);
		if (buf[iter])
			iter++;
	}
	return (exp_str);
}

int	check_envs(char *buf, t_input *data, t_expand_str *str)
{
	size_t			iter;
	t_expand_helper	exh;

	exh.env_iter = 0;
	exh.env_pos_iter = 0;
	iter = 0;
	data->dbl_quote = 0;
	data->sgl_quote = 0;
	while (buf[iter])
	{
		exh.len = envlen(&buf[iter + 1]);
		if ((!quote_check(iter, buf, data) || str->flag == HERE_DOC) && buf[iter] == '$' && exh.len
			&& str->var_count)
		{
			exh.env_return = get_env(&buf[iter + 1], str, &exh,
					data->envp.vars);
			if (check_return_get_env(iter, str, &exh, data) == -1)
				return (-1);
			iter += exh.len;
		}
		else
			data->len++;
		iter++;
	}
	return (0);
}

int	expand_init(t_entry *cur, t_input *data, t_expand_str *str)
{
	size_t	iter;

	iter = 0;
	data->dbl_quote = 0;
	data->sgl_quote = 0;
	while (cur->raw_entry[iter])
	{
		if ((!quote_check(iter, cur->raw_entry, data) || str->flag == HERE_DOC)
			&& cur->raw_entry[iter] == '$')
		{
			if (envlen(&cur->raw_entry[iter + 1]) > 0)
				str->var_count++;
		}
		iter++;
	}
	if (str->var_count > 0)
	{
		str->env_arr = ft_calloc(sizeof(char *), str->var_count + 1);
		if (!str->env_arr)
			return (perror("expand_init"), -1);
		str->env_pos = malloc(sizeof(size_t) * str->var_count * 2);
		if (!str->env_pos)
			return (perror("expand_init"), free(str->env_arr), -1);
	}
	return (0);
}
