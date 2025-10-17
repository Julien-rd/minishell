/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:05:54 by jromann           #+#    #+#             */
/*   Updated: 2025/10/17 16:59:30 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	initialise_data(t_ex_arr *ex_s, t_expand_helper *exh,
		t_entry *current, t_expand_str *str)
{
	exh->env_iter = 0;
	exh->env_pos_iter = 0;
	exh->str_iter = 0;
	exh->buf = current->unquoted;
	ex_s->exp_str = ft_calloc(sizeof(char), str->len + 1);
	ex_s->new_quote = ft_calloc(sizeof(char), str->len + 1);
	if (!ex_s->new_quote)
		return (perror("initialise_data"), free(ex_s->exp_str),
			ex_s->exp_str = NULL, -1);
	return (0);
}

int	expanded_str(t_entry *current, t_ex_arr *ex_s, t_expand_str *str)
{
	size_t			iter;
	t_expand_helper	exh;

	if (initialise_data(ex_s, &exh, current, str) == -1)
		return (-1);
	iter = 0;
	while (ex_s->new_quote && ex_s->exp_str && exh.buf[iter])
	{
		exh.quote = current->quotes[iter];
		if (str->var_count * 2 > exh.env_pos_iter
			&& str->env_pos[exh.env_pos_iter] == iter
			&& (current->quotes[iter] != '1' || str->flag == HERE_DOC))
			iter += ex_encounter(ex_s, &exh, str, iter);
		else
		{
			ex_s->exp_str[exh.str_iter] = exh.buf[iter];
			ex_s->new_quote[exh.str_iter++] = exh.quote;
		}
		if (exh.buf[iter])
			iter++;
	}
	return (0);
}

static int	check_envs(t_entry *current, t_sh *sh, t_expand_str *str)
{
	size_t			iter;
	t_expand_helper	exh;

	exh.env_iter = 0;
	exh.env_pos_iter = 0;
	iter = 0;
	while (current->unquoted[iter])
	{
		exh.len = envlen(&current->unquoted[iter + 1]);
		if ((current->quotes[iter] != '1' || str->flag == HERE_DOC)
			&& current->unquoted[iter] == '$' && (exh.len
				|| (current->quotes[iter + 1] != '0' && current->quotes[iter
					+ 1] != 0)) && str->var_count)
		{
			exh.env_return = get_env(&current->unquoted[iter + 1], str, &exh,
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

static int	expand_init(t_entry *current, t_expand_str *str)
{
	size_t	iter;

	iter = 0;
	while (current->unquoted[iter])
	{
		if ((current->quotes[iter] != '1' || str->flag == HERE_DOC)
			&& current->unquoted[iter] == '$')
		{
			if (envlen(&current->unquoted[iter + 1]) > 0
				|| (current->quotes[iter + 1] != '0' && current->quotes[iter
					+ 1] != 0))
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
	t_ex_arr		ex_str;

	str.flag = flag;
	str.len = 0;
	str.var_count = 0;
	str.exit_code = sh->exit_code;
	str.env_arr = NULL;
	str.env_pos = NULL;
	if (expand_init(current, &str) == -1)
		return (NULL);
	current->exp_count = str.var_count;
	if (check_envs(current, sh, &str) == -1)
		return (free2d(&str.env_arr), free(str.env_pos), NULL);
	expanded_str(current, &ex_str, &str);
	if (ex_str.exp_str)
	{
		free(current->quotes);
		current->quotes = ex_str.new_quote;
	}
	return (free2d(&str.env_arr), free(str.env_pos), ex_str.exp_str);
}
