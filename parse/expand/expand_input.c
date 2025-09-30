/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:05:54 by jromann           #+#    #+#             */
/*   Updated: 2025/09/30 18:14:22 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand(t_entry *cur, t_input *data)
{
	char			**env_arr;
	t_expanded_str	str;
	size_t			iter;
	char			*ex_str;

	data->len = 0;
	data->exp_str_malloc = 1;
	str.var_count = 0;
	str.exit_code = data->exit_code;
	str.env_arr = NULL;
	str.env_pos = NULL;
	if (expand_init(cur, data, &str) == -1)
		return (NULL);
	cur->exp_count = str.var_count;
	if (check_envs(cur->raw_entry, data, &str) == -1)
		return (free2d(&str.env_arr), free(str.env_pos), NULL);
	ex_str = expanded_str(cur->raw_entry, data, &str);
	return (free2d(&str.env_arr), free(str.env_pos), ex_str);
}
