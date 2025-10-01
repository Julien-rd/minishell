/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 19:01:35 by jromann           #+#    #+#             */
/*   Updated: 2025/10/01 19:01:45 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env(char **envp, t_exec *data, t_cmd *cmd, int flag)
{
	size_t	iter;

	iter = 0;
	if (!envp)
		child_exit_handle(data, cmd, 1);
	if (flag == -1)
		invalid_option(data, cmd);
	while (envp[iter])
	{
		if (safe_write(1, envp[iter], ft_strlen(envp[iter])) == -1)
			child_exit_handle(data, cmd, 1);
		if (envp[iter][0] != '\0' && safe_write(1, "\n", 1) == -1)
			child_exit_handle(data, cmd, 1);
		iter++;
	}
	child_exit_handle(data, cmd, 0);
}