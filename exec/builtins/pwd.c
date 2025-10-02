/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 20:15:44 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/02 11:29:22 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(t_sh *sh, t_cmd *cmd, int flag)
{
	char	*current_path;

	if (flag == -1)
		return ;
	current_path = getcwd(NULL, 0);
	if (!current_path)
	{
		perror("getcwd");
		child_exit_handle(sh, cmd, 1);
	}
	if (safe_write(1, current_path, ft_strlen(current_path)) == -1)
		return (free(current_path), child_exit_handle(sh, cmd, 1));
	if (safe_write(1, "\n", 1) == -1)
		return (free(current_path), child_exit_handle(sh, cmd, 1));
	return (free(current_path), child_exit_handle(sh, cmd, 0));
}

