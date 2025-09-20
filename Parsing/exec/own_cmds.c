/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   own_cmds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 20:15:44 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/20 18:17:47 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	cd(char *path)
{
	if (chdir(path) == -1)
    	return (perror("chdir failed"), 1);
    return 0;
}

int	own_exit(int exit_code)
{
	write(1, "exit\n", 5);
	exit(0);
}

int	echo(char **cmd, int nflag)
{
	size_t	iter;

	iter = 1;
	while (cmd[iter])
	{
		if (write(1, cmd[iter], ft_strlen(cmd[iter])) == -1)
			return (perror("write"), -1);
		if (cmd[iter + 1])
			if (write(1, " ", 1) == -1)
				return (perror("write"), -1);
		if (!cmd[iter + 1] && nflag == 0)
			if (write(1, "\n", 1) == -1)
				return (perror("write"), -1);
		iter++;
	}
	return (0);
}

int	export(char **cmd, t_exec *data)
{
	char	*value;
	size_t	iter;

	if (!cmd[1] || cmd[2] != NULL || !ft_strchr(cmd[1], '=') 
	|| cmd[1][0] == '=' || (!ft_isalpha(cmd[1][0]) && cmd[1][0] != '_'))
		return (/* own error */-1);
	iter = 0;
	while (cmd[1][iter] != '=')
	{
		if (!ft_isalnum(cmd[1][iter]) && cmd[1][iter] != '_')
			return (/* own error */-1);
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

