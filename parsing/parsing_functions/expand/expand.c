/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:05:54 by jromann           #+#    #+#             */
/*   Updated: 2025/10/09 16:48:09 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expanded_str(char *buf, t_sh *sh, t_expand_str *str)
{
	size_t			iter;
	t_expand_helper	exh;
	char			*exp_str;

	exh.env_iter = 0;
	exh.env_pos_iter = 0;
	exh.str_iter = 0;
	iter = 0;
	sh->dbl_quote = 0;
	sh->sgl_quote = 0;
	exh.buf = buf;
	exp_str = ft_calloc(sizeof(char), str->len + 1);
	while (exp_str && buf[iter])
	{
		toggle_quotes(buf, sh, iter);
		if (str->var_count * 2 > exh.env_pos_iter
			&& str->env_pos[exh.env_pos_iter] == iter && (sh->sgl_quote == 0
				|| str->flag == HERE_DOC))
			iter += ex_encounter(&exp_str[exh.str_iter], &exh, str, iter);
		else
			exp_str[exh.str_iter++] = buf[iter];
		if (buf[iter])
			iter++;
	}
	return (exp_str);
}

static int	check_envs(char *buf, t_sh *sh, t_expand_str *str)
{
	size_t			iter;
	t_expand_helper	exh;

	exh.env_iter = 0;
	exh.env_pos_iter = 0;
	iter = 0;
	sh->dbl_quote = 0;
	sh->sgl_quote = 0;
	while (buf[iter])
	{
		exh.len = envlen(&buf[iter + 1]);
		if ((!quote_check(iter, buf, sh) || str->flag == HERE_DOC)
			&& buf[iter] == '$' && exh.len && str->var_count)
		{
			exh.env_return = get_env(&buf[iter + 1], str, &exh,
					sh->envp.vars);
			if (check_return_get_env(iter, str, &exh) == -1)
				return (-1);
			iter += exh.len;
		}
		else
			str->len++;
		iter++;
	}
	return (0);
}

static int	expand_init(t_entry *current, t_sh *sh, t_expand_str *str)
{
	size_t	iter;

	iter = 0;
	sh->dbl_quote = 0;
	sh->sgl_quote = 0;
	while (current->raw_entry[iter])
	{
		if ((!quote_check(iter, current->raw_entry, sh)
				|| str->flag == HERE_DOC) && current->raw_entry[iter] == '$')
		{
			if (envlen(&current->raw_entry[iter + 1]) > 0)
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

char	*expand(t_entry *current, t_sh *sh, int flag)
{
	t_expand_str	str;
	char			*exp_str;

	str.flag = flag;
	str.len = 0;
	str.var_count = 0;
	str.exit_code = sh->exit_code;
	str.env_arr = NULL;
	str.env_pos = NULL;
	if (expand_init(current, sh, &str) == -1)
		return (NULL);
	current->exp_count = str.var_count;
	if (check_envs(current->raw_entry, sh, &str) == -1)
		return (free2d(&str.env_arr), free(str.env_pos), NULL);
	exp_str = expanded_str(current->raw_entry, sh, &str);
	return (free2d(&str.env_arr), free(str.env_pos), exp_str);
}
