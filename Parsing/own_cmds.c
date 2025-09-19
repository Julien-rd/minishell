/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   own_cmds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 20:15:44 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/19 21:20:33 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_stringlen(char *str)
{
	size_t	iter;

	iter = 0;
	while (str[iter])
	{
		iter++;
	}
	return (iter);
}

int	cd(char *path)
{
	if (chdir(path) == -1)
    	return (perror("chdir failed"), 1);
    return 0;
}

int	echo(char **cmd, int nflag)
{
	size_t	iter;

	iter = 1;
	while (cmd[iter])
	{
		if (write(1, cmd[iter], ft_stringlen(cmd[iter])) == -1)
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
		if (write(1, envp[iter], ft_stringlen(envp[iter])) == -1)
			return (perror("write"), -1);
		if (write(1, "\n", 1) == -1)
			return (perror("write"), -1);
		iter++;
	}
	return (0);
}

int main(int argc, char *argv[], char **envp)
{
	//cd(argv[1]);
	// echo(argv, 1);
	env(envp);
	return (0);
}

