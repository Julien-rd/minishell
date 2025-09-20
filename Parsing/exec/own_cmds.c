/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   own_cmds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 20:15:44 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/20 12:11:51 by eprottun         ###   ########.fr       */
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

