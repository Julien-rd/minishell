/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:05:54 by jromann           #+#    #+#             */
/*   Updated: 2025/10/02 11:29:22 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand(t_entry *cur, t_sh *sh, int flag)
{
	char			**env_arr;
	t_expand_str	str;
	size_t			iter;
	char			*exp_str;

	str.flag = flag;
	str.len = 0;
	str.var_count = 0;
	str.exit_code = sh->exit_code;
	str.env_arr = NULL;
	str.env_pos = NULL;
	if (expand_init(cur, sh, &str) == -1)
		return (NULL);
	cur->exp_count = str.var_count;
	if (check_envs(cur->raw_entry, sh, &str) == -1)
		return (free2d(&str.env_arr), free(str.env_pos), NULL);
	exp_str = expanded_str(cur->raw_entry, sh, &str);
	return (free2d(&str.env_arr), free(str.env_pos), exp_str);
}
