/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 15:16:59 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/14 16:18:39 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	insert_pos(t_sh *sh, char *param)
{
	size_t	iter;
	size_t	end_of_name;

	end_of_name = 0;
	while (param[end_of_name] != '=')
		end_of_name++;
	iter = 0;
	while (sh->envp.vars[iter])
	{
		if (!ft_strncmp(sh->envp.vars[iter], param, end_of_name + 1))
			return (iter);
		iter++;
	}
	iter = 0;
	while (sh->envp.vars[iter])
	{
		if (!ft_strcmp(sh->envp.vars[iter], ""))
			return (iter);
		iter++;
	}
	return (-1);
}

int	extend_envp(t_sh *sh)
{
	char	**new_envp;
	size_t	iter;
	size_t	new_iter;

	iter = 0;
	new_iter = 0;
	new_envp = malloc(sizeof(char *) * ((int)(sh->envp.malloc * 1.5) + 1));
	if (!new_envp)
		return (perror("extend_envp"), -1);
	sh->envp.malloc = (size_t)(sh->envp.malloc * 1.5);
	while (sh->envp.vars[iter])
	{
		if (!ft_strcmp(sh->envp.vars[iter], ""))
		{
			free(sh->envp.vars[iter]);
			sh->envp.count--;
		}
		else
			new_envp[new_iter++] = sh->envp.vars[iter];
		iter++;
	}
	new_envp[new_iter] = NULL;
	free(sh->envp.vars);
	sh->envp.vars = new_envp;
	return (0);
}

char	*cut_plus(char *entry)
{
	char	*new;

	new = malloc(ft_strlen(entry));
	if (!new)
		return (perror("cut_plus"), NULL);
	ft_strlcpy(new, entry, envlen(entry) + 1);
	ft_strlcat(new, &entry[envlen(entry) + 1], ft_strlen(entry) + 1);
	return (new);
}
