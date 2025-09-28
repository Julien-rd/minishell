/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   own_cmds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 20:15:44 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/28 12:31:52 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(t_exec *data, t_cmd *cmd, int flag)
{
	char	*buf;

	if (flag == -1)
		return ;
	buf = getcwd(NULL, 0);
	if (!buf)
	{
		perror("getcwd");
		child_exit_handle(data, cmd, 1);
	}
	if (safe_write(1, buf, ft_strlen(buf)) == -1)
		return (free(buf), child_exit_handle(data, cmd, 1));
	if (safe_write(1, "\n", 1) == -1)
		return (free(buf), child_exit_handle(data, cmd, 1));
	return (free(buf), child_exit_handle(data, cmd, 0));
}

int	exit_cmd(t_exec *data, t_cmd *cmd)
{
	if (!data->pipe_count)
		data->exit = 1;
	return (0);
}

void	echo(t_exec *data, t_cmd *cmd, int nflag)
{
	size_t	iter;

	iter = 1;
	if (nflag == -1)
		return ;
	iter += nflag;
	while (cmd->cmd[iter])
	{
		if (safe_write(1, cmd->cmd[iter], ft_strlen(cmd->cmd[iter])) == -1)
			child_exit_handle(data, cmd, 1);
		if (cmd->cmd[iter + 1])
		{
			if (safe_write(1, " ", 1) == -1)
				child_exit_handle(data, cmd, 1);
		}
		iter++;
	}
	if (nflag == 0)
	{
		if (safe_write(1, "\n", 1) == -1)
			child_exit_handle(data, cmd, 1);
	}
	child_exit_handle(data, cmd, 0);
}

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
