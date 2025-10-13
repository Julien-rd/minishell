/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 14:58:44 by jromann           #+#    #+#             */
/*   Updated: 2025/10/13 17:08:32 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ex_encounter(char *str_new, t_expand_helper *exh, t_expand_str *str,
		size_t iter)
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

int	check_return_get_env(size_t iter, t_expand_str *str, t_expand_helper *exh)
{
	if (exh->env_return == 1)
	{
		str->len += ft_strlen(str->env_arr[exh->env_iter]);
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

char	*remove_quotes(char *to_strip, size_t len)
{
	size_t	iter;
	size_t	stripped_iter;
	t_sh	sh;
	char	*stripped;

	stripped_iter = 0;
	sh.dbl_quote = 0;
	sh.sgl_quote = 0;
	stripped = malloc((len + 1) * sizeof(char));
	if (!stripped)
		return (NULL);
	iter = 0;
	while (iter < len)
	{
		while (toggle_quotes(to_strip, &sh, iter))
			iter++;
		if (iter >= len)
			break ;
		if (!(to_strip[iter] == '\'' && sh.sgl_quote)
			&& !(to_strip[iter] == '\"' && sh.dbl_quote))
			stripped[stripped_iter++] = to_strip[iter];
		iter++;
	}
	stripped[stripped_iter] = '\0';
	return (stripped);
}
