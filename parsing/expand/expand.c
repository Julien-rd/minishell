/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:05:54 by jromann           #+#    #+#             */
/*   Updated: 2025/10/02 10:39:10 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand(t_entry *cur, t_input *data, int flag)
{
	char			**env_arr;
	t_expand_str	str;
	size_t			iter;
	char			*exp_str;

	str.flag = flag;
	str.len = 0;
	str.var_count = 0;
	str.exit_code = data->exit_code;
	str.env_arr = NULL;
	str.env_pos = NULL;
	if (expand_init(cur, data, &str) == -1)
		return (NULL);
	cur->exp_count = str.var_count;
	if (check_envs(cur->raw_entry, data, &str) == -1)
		return (free2d(&str.env_arr), free(str.env_pos), NULL);
	exp_str = expanded_str(cur->raw_entry, data, &str);
	return (free2d(&str.env_arr), free(str.env_pos), exp_str);
}
