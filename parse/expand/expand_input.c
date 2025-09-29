/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:05:54 by jromann           #+#    #+#             */
/*   Updated: 2025/09/29 13:29:16 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand(char *buf, t_input *data)
{
	char			**env_arr;
	t_expanded_str	str;
	t_expand_helper	exh;
	size_t			iter;

	(void)exh;
	data->len = 0;
	data->exp_str_malloc = 1;
	str.var_count = 0;
	str.exit_code = data->exit_code;
	str.env_arr = NULL;
	str.env_pos = NULL;
	if (expand_init(buf, data, &str) == -1)
		return (-1);
	if (check_envs(buf, data, &str) == -1)
		return (free2d(&str.env_arr), free(str.env_pos), -1);
	if (expanded_str(buf, data, &str) == -1)
		return (free2d(&str.env_arr), free(str.env_pos), -1);
	return (free2d(&str.env_arr), free(str.env_pos), 0);
}
