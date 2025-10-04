/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:45:11 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/04 12:44:22 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	update_pwd_entry(char **dest, char *pwd_str, char *cwd_str)
{
	char	*new_entry;

	new_entry = ft_strjoin(pwd_str, cwd_str);
	if (!new_entry)
		return (perror("update_pwd_entry"), -1);
	free(*dest);
	*dest = new_entry;
	return (0);
}

int	envp_pwd(t_sh *sh, char *tmp_cwd)
{
	int		position;
	char	*cwd;
	size_t	iter;

	iter = 0;
	while (sh->envp.vars[iter])
	{
		if (!ft_strncmp(sh->envp.vars[iter], "OLDPWD=", 7))
		{
			if (update_pwd_entry(&sh->envp.vars[iter], "OLDPWD=", tmp_cwd)
				== -1)
				return (-1);
		}
		if (!ft_strncmp(sh->envp.vars[iter], "PWD=", 4))
		{
			cwd = getcwd(NULL, 0);
			if (!cwd)
				return (perror("cd"), -1);
			if (update_pwd_entry(&sh->envp.vars[iter], "PWD=", cwd) == -1)
				return (free(cwd), -1);
			free(cwd);
		}
		iter++;
	}
	return (0);
}

int	cd(t_sh *sh, char **argv, size_t pipe_count)
{
	size_t	iter;
	char	*tmp_cwd;

	iter = 0;
	while (argv[iter])
		iter++;
	if (iter > 2)
		return (-2);
	if (pipe_count)
		return (0);
	tmp_cwd = getcwd(NULL, 0);
	if (!tmp_cwd)
		return (perror("cd"), -1);
	if (!pipe_count && chdir(argv[1]) == -1)
	{
		sh->exit_code = errno;
		return (free(tmp_cwd), -1);
	}
	if (envp_pwd(sh, tmp_cwd) == -1)
		return (free(tmp_cwd), perror("cd"), -1);
	free(tmp_cwd);
	return (0);
}
