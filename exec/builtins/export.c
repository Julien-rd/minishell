/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:28:10 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/14 15:35:27 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	input_check(char *param)
{
	size_t	iter;

	if (!ft_isalpha(param[0]) && param[0] != '_')
		return (-1);
	iter = 1;
	while (param[iter] && param[iter] != '=')
	{
		if (param[iter] == '+' && param[iter + 1] == '=')
			return (1);
		else if (!ft_isalnum(param[iter]) && param[iter] != '_')
			return (-1);
		iter++;
	}
	return (0);
}

static int	insert_env(t_sh *sh, char *entry)
{
	char	*new;
	int		position;

	new = ft_strdup(entry);
	if (!new)
		return (perror("insert_env"), -1);
	position = insert_pos(sh, entry);
	if (position == -1)
	{
		if (sh->envp.count >= sh->envp.malloc)
			if (extend_envp(sh) == -1)
				return (perror("insert_env"), free(new), -1);
		sh->envp.vars[sh->envp.count] = new;
		sh->envp.count = sh->envp.count + 1;
		sh->envp.vars[sh->envp.count] = NULL;
	}
	else
	{
		free(sh->envp.vars[position]);
		sh->envp.vars[position] = new;
	}
	return (0);
}

int	add_env(t_sh *sh, char *entry)
{
	char	*new;
	int		position;

	new = cut_plus(entry);
	if (!new)
		return (-1);
	position = insert_pos(sh, new);
	if (position == -1)
	{
		if (sh->envp.count >= sh->envp.malloc)
			if (extend_envp(sh) == -1)
				return (perror("add_env"), free(new), -1);
		sh->envp.vars[sh->envp.count++] = new;
		sh->envp.vars[sh->envp.count] = NULL;
	}
	else
	{
		free(new);
		new = ft_strjoin(sh->envp.vars[position], &entry[envlen(entry) + 2]);
		if (!new)
			return (perror("add_env"), -1);
		free(sh->envp.vars[position]);
		sh->envp.vars[position] = new;
	}
	return (0);
}

int	export(char **argv, t_pipeline *pl, t_sh *sh)
{
	size_t	iter;
	int		input_type;
	
	iter = 1;
	if (pl->count)
		return (0);
	while (argv[iter])
	{
		input_type = input_check(argv[iter]);
		if (input_type == 0 && ft_strchr(argv[iter], '=')
			&& insert_env(sh, argv[iter]) == -1)
			return (-1);
		else if (input_type == 1 && add_env(sh, argv[iter]) == -1)
			return (-1);
		iter++;
	}
	return (0);
}
