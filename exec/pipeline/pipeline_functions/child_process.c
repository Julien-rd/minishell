/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 14:33:26 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/25 14:44:15 by eprottun         ###   ########.fr       */
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

	if (ft_strchr(pl->current->argv[0], '/')
		|| ft_find_paths(sh->envp.vars, "PATH") != -1)
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
			return (perror("no_path_execution"),
				free(full_path), free(cwd), -1);
		return (free(full_path), free(cwd), 1);
	}
	free(full_path);
	free(cwd);
	return (2);
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
	execve(path, pl->current->argv, sh->envp.vars);
	execve_fail(path, no_path, pl, sh);
}
