/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 12:15:30 by jromann           #+#    #+#             */
/*   Updated: 2025/10/22 11:12:52 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	safe_write(int fd, char *buf, size_t len)
{
	if (write(fd, buf, len) == -1)
		return (perror("write"), -1);
	return (0);
}

int	is_token(char c)
{
	if (c == 32 || (c >= 9 && c <= 13) || c == '>' || c == '<' || c == '|')
		return (0);
	return (1);
}

size_t	skip_whitspaces(char *buf)
{
	size_t	iter;

	iter = 0;
	while ((buf[iter] >= 9 && buf[iter] <= 13) || buf[iter] == 32)
		iter++;
	return (iter);
}

bool	empty_prompt(char *buf)
{
	if (ft_strlen(buf) == 0)
		return (true);
	return (false);
}

char	*env_var(char *var_name, t_sh *sh)
{
	size_t	iter;

	iter = 0;
	while (sh->envp.vars[iter])
	{
		if (!ft_strncmp(var_name, sh->envp.vars[iter], ft_strlen(var_name))
			&& sh->envp.vars[iter][ft_strlen(var_name)] == '=')
			return (&sh->envp.vars[iter][ft_strlen(var_name) + 1]);
		iter++;
	}
	return (NULL);
}
