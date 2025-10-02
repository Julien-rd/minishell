/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:28:10 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/02 11:29:22 by eprottun         ###   ########.fr       */
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
		if (!ft_isalnum(param[iter]) && param[iter] != '_')
			return (-1);
		iter++;
	}
	return (0);
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

int	insert_env(t_sh *sh, char *entry)
{
	char	*new;
	size_t	position;

	new = ft_strdup(entry);
	if (!new)
		return (perror("export"), -1);
	position = insert_pos(sh, entry);
	if (position == -1)
	{
		if (sh->envp.count >= sh->envp.malloc)
			if (extend_envp(sh) == -1)
				return (perror("export"), free(new), -1);
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

int	export(char **cmd, t_sh *sh)
{
	size_t	iter;
	int		return_value;

	iter = 1;
	return_value = 0;
	while (cmd[iter])
	{
		if (input_check(cmd[iter]) == -1)
		{
			write(1, "export: `", 10);
			write(1, cmd[iter], ft_strlen(cmd[iter]));
			write(1, "': not a valid identifier\n", 26);
			iter++;
			return_value = 1;
			continue ;
		}
		if (!ft_strchr(cmd[iter], '='))
		{
			iter++;
			continue ;
		}
		if (insert_env(sh, cmd[iter]) == -1)
			return (-1);
		iter++;
	}
	return (return_value);
}
