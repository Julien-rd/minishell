/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   own_cmds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 20:15:44 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/22 10:25:12 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	cd(t_cmd *cmd)
{
	size_t	iter;

	while (cmd->cmd[iter])
		iter++;
	if (iter > 2)
	{
		write(2, "cd: too many arguments", 22);
		return (1);
	}
	else if (chdir(cmd->cmd[1]) == -1)
		return (perror("cd"), 1);
	// Error handling chdir kackt ab// path not found
	return (0);
}
int	pwd(void)
{
	char	*buf;

	buf = getcwd(NULL, 0);
	if (!buf)
		return (-1);
	write(1, buf, ft_strlen(buf));
	return 0;
}

int	exit_cmd(t_exec *data, t_cmd *cmd)
{
	size_t	iter;

	iter = 1;
	if (cmd->cmd[1] == NULL && data->pipe_count == 0)
	{
		write(1, "exit", 5);
		exit(12);
	}
	else if (cmd->cmd[1] == NULL)
		exit(0);
	else
	{
		write(2, "exit : invalid option --", 24);
		while (cmd->cmd[iter])
		{
			write(2, " ", 1);
			write(2, cmd->cmd[iter], ft_strlen(cmd->cmd[iter]));
			iter++;
		}
		write(2, "\n", 1);
		exit(2); // exit failed in pipe error code 2
	}
}

int	echo(char **cmd, int nflag)
{
	size_t	iter;

	iter = 1;
	if (nflag == 2)
		iter = 2;
	while (cmd[iter])
	{
		if (write(1, cmd[iter], ft_strlen(cmd[iter])) == -1)
			return (perror("write"), -1);
		if (cmd[iter + 1])
			if (write(1, " ", 1) == -1)
				return (perror("write"), -1);
		iter++;
	}
	if (nflag == 2)
		if (write(1, "\n", 1) == -1)
			return (perror("write"), -1);
	return (0);
}

int	export(char **cmd, t_exec *data)
{
	char	*value;
	size_t	iter;

	if (!cmd[1] || cmd[2] != NULL || !ft_strchr(cmd[1], '=') || cmd[1][0] == '='
		|| (!ft_isalpha(cmd[1][0]) && cmd[1][0] != '_'))
		return (/* own error */ -1);
	iter = 0;
	while (cmd[1][iter] != '=')
	{
		if (!ft_isalnum(cmd[1][iter]) && cmd[1][iter] != '_')
			return (/* own error */ -1);
		iter++;
	}
	while (cmd[1][iter])
		iter++;
	value = ft_strdup(cmd[1]);
	if (!value)
		return (perror("export"), -1);
	if (data->envp_count >= data->envp_malloc)
		if (!extend_envp(data))
			return (free(value), perror("export"), -1);
	data->envp[data->envp_count] = value;
	data->envp[++data->envp_count] = NULL;
	return (0);
}

int	env(char **envp)
{
	size_t	iter;

	iter = 0;
	if (!envp)
		return (-1);
	while (envp[iter])
	{
		if (write(1, envp[iter], ft_strlen(envp[iter])) == -1)
			return (perror("write"), -1);
		if (write(1, "\n", 1) == -1)
			return (perror("write"), -1);
		iter++;
	}
	return (0);
}

