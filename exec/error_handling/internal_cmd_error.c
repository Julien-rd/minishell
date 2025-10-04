/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_cmd_error.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 09:09:33 by jromann           #+#    #+#             */
/*   Updated: 2025/10/04 19:19:31 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_error_msg(t_pipeline *pl, t_sh *sh, int flag)
{
	if (flag == NUMERIC_ARG)
	{
		if (safe_write(2, "exit: ", 6) == -1)
			child_exit_handle(sh, pl, NULL, 1);
		if (safe_write(2, pl->current->argv[1],
				ft_strlen(pl->current->argv[1])) == -1)
			child_exit_handle(sh, pl, NULL, 1);
		if (safe_write(2, ": numeric argument required\n", 28) == -1)
			child_exit_handle(sh, pl, NULL, 1);
		child_exit_handle(sh, pl, NULL, 2);
	}
	else if (flag == TOO_MANY_ARGS)
	{
		safe_write(2, "exit: too many arguments\n", 25);
		child_exit_handle(sh, pl, NULL, 1);
	}
}

void	exit_error(t_pipeline *pl, t_sh *sh)
{
	long long	exit_code;
	size_t		iter;

	iter = 0;
	if (!pl->current->argv[1])
		child_exit_handle(sh, pl, NULL, sh->exit_code);
	if (pl->current->argv[1][0] == '0' && pl->current->argv[1][1] == '\0')
	{
		if (pl->current->argv[2])
			exit_error_msg(pl, sh, TOO_MANY_ARGS);
		child_exit_handle(sh, pl, NULL, 0);
	}
	exit_code = ft_atoll(pl->current->argv[1]);
	if (exit_code == 0)
		exit_error_msg(pl, sh, NUMERIC_ARG);
	if (pl->current->argv[2])
		exit_error_msg(pl, sh, TOO_MANY_ARGS);
	child_exit_handle(sh, pl, NULL, exit_code);
}

void	export_unset_error(t_pipeline *pl, t_sh *sh, int cmd_flag)
{
	if (cmd_flag == EXPORT && sh->internal_errcode == -1)
	{
		write(2, "export: malloc failed\n", 22);
		child_exit_handle(sh, pl, NULL, 1);
	}
	if (cmd_flag == EXPORT && sh->internal_errcode == 1)
		child_exit_handle(sh, pl, NULL, 1);
	else if (cmd_flag == UNSET && sh->internal_errcode == 1)
		write(2, "unset: malloc failed\n", 21);
	child_exit_handle(sh, pl, NULL, 0);
}

void	cd_error(t_cmd *cur, t_pipeline *pl, t_sh *sh)
{
	if (sh->internal_errcode == -2)
	{
		write(2, "cd: too many arguments\n", 23);
		child_exit_handle(sh, pl, NULL, 1);
	}
	else if (!pl->count)
	{
		if (sh->internal_errcode != 0)
		{
			write(2, "cd: ", 4);
			errno = sh->exit_code;
			perror(cur->argv[1]);
			child_exit_handle(sh, pl, NULL, 1);
		}
	}
	else if (chdir(cur->argv[1]) == -1)
	{
		write(2, "cd: ", 4);
		perror(cur->argv[1]);
		child_exit_handle(sh, pl, NULL, 1);
	}
	child_exit_handle(sh, pl, NULL, 0);
}

void	internal_cmd_error(t_pipeline *pl, t_sh *sh, int flag)
{
	size_t		iter;

	iter = 0;
	if (flag == -1 && pl->current->cmd_flag != EXIT)
		invalid_option(pl, sh);
	if (pl->current->cmd_flag == CD)
		cd_error(pl->current, pl, sh);
	if (pl->current->cmd_flag == EXPORT || pl->current->cmd_flag == UNSET)
		export_unset_error(pl, sh, pl->current->cmd_flag);
	if (pl->current->cmd_flag == EXIT)
		exit_error(pl, sh);
	child_exit_handle(sh, pl, NULL, 0);
}
