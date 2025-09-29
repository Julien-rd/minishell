/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanded_input_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 14:58:44 by jromann           #+#    #+#             */
/*   Updated: 2025/09/29 16:25:00 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ex_encounter(char *str_new, t_expand_helper *exh,
		t_expanded_str *str, size_t iter, int *input_spec)
{
	if (str->env_pos[exh->env_pos_iter + 1] == 0)
	{
		if (input_spec[0] == HERE_DOC)
			input_spec[0] = A_HERE_DOC;
		if (input_spec[0] == APPEND_FILE)
			input_spec[0] = A_APPEND;
		if (input_spec[0] == INFILE)
			input_spec[0] = A_INFILE;
		if (input_spec[0] == OUTFILE)
			input_spec[0] = A_OUTFILE;
		if (input_spec[0] == DEFAULT)
			input_spec[0] = NULL_DEFAULT;
		exh->env_pos_iter += 2;
	}
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

char	*expanded_str(char *buf, t_input *data, t_expanded_str *str, int *input_spec)
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
	if (!exp_str)
		return (NULL);
	while (buf[iter])
	{
		iter += toggle_quotes(buf, data, iter);
		if (str->var_count * 2 > exh.env_pos_iter
			&& str->env_pos[exh.env_pos_iter] == iter)
			iter += ex_encounter(&exp_str[exh.str_iter], &exh, str, iter, input_spec);
		else if (!(data->sgl_quote == 1 && buf[iter] == '\'')
				&& !(data->dbl_quote == 1 && buf[iter] == '\"'))
			exp_str[exh.str_iter++] = buf[iter];
		toggle_quotes(buf, data, iter);
		if (buf[iter])
			iter++;
	}
	return (exp_str);
}

static int	check_return_get_env(size_t iter, t_expanded_str *str,
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

int	check_envs(char *buf, t_input *data, t_expanded_str *str)
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
		if (!quote_check(iter, buf, data) && buf[iter] == '$' && exh.len
			&& str->var_count)
		{
			exh.env_return = get_env(&buf[iter + 1], str, &exh, data->envp);
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

int	expand_init(char *buf, t_input *data, t_expanded_str *str)
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
		str->env_arr = ft_calloc(sizeof(char *), str->var_count + 1);
		if (!str->env_arr)
			return (perror("expand_init"), -1);
		str->env_pos = malloc(sizeof(size_t) * str->var_count * 2);
		if (!str->env_pos)
			return (perror("expand_init"), free(str->env_arr), -1);
	}
	return (0);
}
