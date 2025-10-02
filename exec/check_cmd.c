/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 08:36:18 by jromann           #+#    #+#             */
/*   Updated: 2025/10/02 11:29:22 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	options_check(t_cmd *cmd)
{
	size_t	iter;
	size_t	n_iter;

	iter = 1;
	if (!cmd)
		return (-1);
	if (!ft_strncmp(cmd->cmd[0], "echo", 5) && cmd->cmd[1])
	{
		while (cmd->cmd[iter] && cmd->cmd[iter][0] == '-')
		{
			n_iter = 1;
			while (cmd->cmd[iter][n_iter] == 'n')
				n_iter++;
			if (cmd->cmd[iter][n_iter])
				break ;
			iter++;
		}
		if (ft_strncmp(cmd->cmd[iter], "-e", 3) && ft_strncmp(cmd->cmd[iter],
				"-E", 3))
			return (-1);
		return (iter - 1);
	}
	if (cmd->cmd[1] != NULL && cmd->cmd[1][0] == '-' && cmd->cmd[1][1])
		return (-1);
	return (0);
}

void	cmd_flag(t_sh *sh, t_cmd *cmd)
{
	cmd->cmd_flag = EXTERNAL;
	sh->internal_errcode = 0;
	if (!ft_strncmp(cmd->cmd[0], "echo", 5))
		cmd->cmd_flag = ECHO;
	else if (!ft_strncmp(cmd->cmd[0], "cd", 3))
		cmd->cmd_flag = CD;
	else if (!ft_strncmp(cmd->cmd[0], "pwd", 4))
		cmd->cmd_flag = PWD;
	else if (!ft_strncmp(cmd->cmd[0], "export", 7))
		cmd->cmd_flag = EXPORT;
	else if (!ft_strncmp(cmd->cmd[0], "unset", 6))
		cmd->cmd_flag = UNSET;
	else if (!ft_strncmp(cmd->cmd[0], "env", 4))
		cmd->cmd_flag = ENV;
	else if (!ft_strncmp(cmd->cmd[0], "exit", 5))
		cmd->cmd_flag = EXIT;
}
