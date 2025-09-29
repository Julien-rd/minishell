/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:05:54 by jromann           #+#    #+#             */
/*   Updated: 2025/09/29 12:58:31 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *expand(char *buf, t_input *data)
{
	char			**env_arr;
	t_expanded_str	str;
	size_t			iter;
	char *ex_str;

	data->len = 0;
	data->exp_str_malloc = 1;
	str.var_count = 0;
	str.exit_code = data->exit_code;
	str.env_arr = NULL;
	if (expand_init(buf, data, &str) == -1)
		return (free(buf), NULL);
	if (check_envs(buf, data, &str) == -1)
		return (free(buf), free2d(&str.env_arr), free(str.env_pos), NULL);
	ex_str = expanded_str(buf, data, &str);
	return (free(buf), free2d(&str.env_arr), free(str.env_pos), ex_str);
}
