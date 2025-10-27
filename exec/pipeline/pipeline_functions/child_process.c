/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 14:33:26 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/27 12:06:54 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	builtin_handler(t_pipeline *pl, t_sh *sh)
{
	int	flag;

	flag = options_check(pl->current);
	if (pl->current->cmd_flag == EXPORT)
		print_declare_x(pl, sh);
	if (pl->current->cmd_flag == ECHO)
		echo(pl, sh, flag);
	else if (pl->current->cmd_flag == PWD)
		pwd(sh, pl, flag);
	else if (pl->current->cmd_flag == ENV)
		env(pl, sh, flag);
	else
		internal_cmd_error(pl, sh, flag);
}

static int	no_path_execution(char **path, t_pipeline *pl, t_sh *sh)
{
	char	*cwd;
	char	*full_path;

	if (ft_strchr(pl->current->argv[0], '/') || ft_find_paths(sh->envp.vars,
			"PATH") != -1)
		return (0);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (perror("getcwd"), -1);
	full_path = ft_strjointhree(cwd, "/", pl->current->argv[0]);
	if (!full_path)
		return (perror("no_path_execution"), free(cwd), -1);
	if (access(full_path, F_OK) == 0)
	{
		*path = ft_strjoin("./", pl->current->argv[0]);
		if (!*path)
			return (perror("no_path_execution"), free(full_path), free(cwd),
				-1);
		return (free(full_path), free(cwd), 1);
	}
	free(full_path);
	free(cwd);
	return (2);
}

static int	shlvl(t_sh *sh)
{
	size_t		iter;
	long long	shlvl_num;
	char		*new_shlvl;

	iter = 0;
	while (sh->envp.vars[iter] && ft_strncmp("SHLVL=", sh->envp.vars[iter], 6))
		iter++;
	if (!sh->envp.vars[iter] || !is_a_num(&sh->envp.vars[iter][6]))
		return (0);
	shlvl_num = ft_atoll(&sh->envp.vars[iter][6]) + 1;
	if (shlvl_num <= 1 || shlvl_num > INT_MAX)
		return (0);
	new_shlvl = ft_itoa(shlvl_num);
	if (!new_shlvl)
		return (perror("shlvl"), -1);
	free(sh->envp.vars[iter]);
	sh->envp.vars[iter] = ft_strjoin("SHLVL=", new_shlvl);
	if (!sh->envp.vars[iter])
		return (perror("shlvl"), free(new_shlvl), -1);
	return (free(new_shlvl), 0);
}

void	child_process(t_pipeline *pl, t_sh *sh)
{
	char	*path;
	int		no_path;

	setup_child_signals();
	if (setup_redirect(sh, pl) == -1)
		child_exit(sh, pl, 1);
	if (pl->current->argv[0] == NULL)
		child_exit(sh, pl, 0);
	if (pl->current->cmd_flag != EXTERNAL)
		builtin_handler(pl, sh);
	no_path = no_path_execution(&path, pl, sh);
	if (no_path == -1)
		child_exit(sh, pl, 1);
	if (!no_path || no_path == 2)
		path = ft_getpath(sh->envp.vars, pl->current->argv[0]);
	if (path == NULL || pl->current->argv[0][0] == 0)
	{
		if ((path && !ft_strchr(pl->current->argv[0], '/')) || no_path == 1)
			free(path);
		command_fail(pl, sh);
	}
	shlvl(sh);
	execve(path, pl->current->argv, sh->envp.vars);
	execve_fail(path, no_path, pl, sh);
}
