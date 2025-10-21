/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_cmd_error.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 09:09:33 by jromann           #+#    #+#             */
/*   Updated: 2025/10/21 16:42:27 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_error_msg(t_pipeline *pl, t_sh *sh, int flag)
{
	if (flag == NUMERIC_ARG)
	{
		if (safe_write(2, "exit: ", 6) == -1)
			child_exit(sh, pl, 1);
		if (safe_write(2, pl->current->argv[1],
				ft_strlen(pl->current->argv[1])) == -1)
			child_exit(sh, pl, 1);
		if (safe_write(2, ": numeric argument required\n", 28) == -1)
			child_exit(sh, pl, 1);
		child_exit(sh, pl, 2);
	}
	else if (flag == TOO_MANY_ARGS)
	{
		safe_write(2, "exit: too many arguments\n", 25);
		child_exit(sh, pl, 1);
	}
}

static void	exit_error(t_pipeline *pl, t_sh *sh)
{
	long long	exit_code;
	size_t		iter;

	iter = 0;
	if (!pl->current->argv[1])
		child_exit(sh, pl, sh->exit_code);
	if (pl->current->argv[1][0] == '0' && pl->current->argv[1][1] == '\0')
	{
		if (pl->current->argv[2])
			exit_error_msg(pl, sh, TOO_MANY_ARGS);
		child_exit(sh, pl, 0);
	}
	exit_code = ft_atoll(pl->current->argv[1]);
	if (exit_code == 0)
		exit_error_msg(pl, sh, NUMERIC_ARG);
	if (pl->current->argv[2])
		exit_error_msg(pl, sh, TOO_MANY_ARGS);
	child_exit(sh, pl, exit_code);
}

static void	export_unset_error(t_pipeline *pl, t_sh *sh, int cmd_flag)
{
	size_t	iter;
	size_t	return_value;

	return_value = 0;
	if (cmd_flag == EXPORT)
	{
		iter = 1;
		while (pl->current->argv[iter])
		{
			if (input_check(pl->current->argv[iter]) == -1)
			{
				return_value = 1;
				if (safe_write(2, "export: `", 10) == -1)
					child_exit(sh, pl, 1);
				if (safe_write(2, pl->current->argv[iter],
						ft_strlen(pl->current->argv[iter])) == -1)
					child_exit(sh, pl, 1);
				if (safe_write(2, "': not a valid identifier\n", 26) == -1)
					child_exit(sh, pl, 1);
			}
			iter++;
		}
		child_exit(sh, pl, return_value);
	}
	child_exit(sh, pl, 0);
}

static void	cd_error(t_cmd *cur, t_pipeline *pl, t_sh *sh)
{
	errno = sh->exit_code;
	if (errno == ENOENT && sh->internal_errcode == 0 && safe_write(2,
			"cd: cannot access current directory, attempting to recover\n",
			59) != -1)
		child_exit(sh, pl, 0);
	else if ((!pl->count && sh->internal_errcode != 0)
		|| sh->internal_errcode == -2)
	{
		if (sh->internal_errcode == -4 && errno == ENOENT)
			safe_write(2, "cd: HOME not set, cannot recover\n", 33);
		else if (sh->internal_errcode == -2)
			safe_write(2, "cd: too many arguments\n", 23);
		else if (cur->argv[1])
			return (perror(cur->argv[1]), child_exit(sh, pl, 1));
		else if (sh->internal_errcode == -3)
			return (perror("cd"), child_exit(sh, pl, 1));
		else if (sh->internal_errcode == -4)
			safe_write(2, "cd: HOME not set\n", 17);
		child_exit(sh, pl, 1);
	}
	else if (pl->count && !cur->argv[1])
		child_exit(sh, pl, cd_no_arg(NULL, sh, !DEFAULT));
	else if (pl->count && cur->argv[1][0] && chdir(cur->argv[1]) == -1)
		return (perror(cur->argv[1]), child_exit(sh, pl, 1));
	child_exit(sh, pl, 0);
}

void	internal_cmd_error(t_pipeline *pl, t_sh *sh, int flag)
{
	size_t	iter;

	iter = 0;
	if (flag == -1 && pl->current->cmd_flag != EXIT)
		invalid_option(pl, sh);
	if (pl->current->cmd_flag == CD)
		cd_error(pl->current, pl, sh);
	if (pl->current->cmd_flag == EXPORT || pl->current->cmd_flag == UNSET)
		export_unset_error(pl, sh, pl->current->cmd_flag);
	if (pl->current->cmd_flag == EXIT)
		exit_error(pl, sh);
	child_exit(sh, pl, 0);
}
