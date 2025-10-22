/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 12:17:18 by jromann           #+#    #+#             */
/*   Updated: 2025/10/22 12:57:36 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_envp(t_sh *sh, char *envp[])
{
	size_t	iter;
	size_t	create_iter;

	iter = 0;
	while (envp[iter])
		iter++;
	sh->envp.count = iter;
	sh->envp.malloc = (iter + (iter == 0)) * 2;
	sh->envp.vars = malloc((sh->envp.malloc + 1) * sizeof(char *));
	if (!sh->envp.vars)
		return (perror("create_envp"), -1);
	create_iter = 0;
	while (create_iter < iter)
	{
		sh->envp.vars[create_iter] = ft_strdup(envp[create_iter]);
		if (!sh->envp.vars[create_iter])
			return (perror("create_envp"), free2d(&sh->envp.vars), -1);
		create_iter++;
	}
	sh->envp.vars[create_iter] = NULL;
	return (0);
}

int	init_shell(t_sh *sh, int argc, char **argv, char **envp)
{
	(void)argv;
	if (argc != 1)
		return (-1);
	if (create_envp(sh, envp) == -1)
		return (-1);
	sh->og_path = getcwd(NULL, 0);
	if (!sh->og_path)
		return (perror("init_shell"), free2d(&sh->envp.vars), -1);
	sh->exit_code = 0;
	sh->exit = 0;
	return (0);
}
