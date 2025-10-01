/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:28:10 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/29 10:33:16 by jromann          ###   ########.fr       */
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

int	extend_envp(t_exec *data)
{
	char	**new_envp;
	size_t	iter;
	size_t	new_iter;

	iter = 0;
	new_iter = 0;
	new_envp = malloc(sizeof(char *) * ((int)(data->envp_malloc * 1.5) + 1));
	if (!new_envp)
		return (perror("extend_envp"), -1);
	data->envp_malloc = (size_t)(data->envp_malloc * 1.5);
	while (data->envp[iter])
	{
		if (!ft_strcmp(data->envp[iter], ""))
		{
			free(data->envp[iter]);
			data->envp_count--;
		}
		else
			new_envp[new_iter++] = data->envp[iter];
		iter++;
	}
	new_envp[new_iter] = NULL;
	free(data->envp);
	data->envp = new_envp;
	return (0);
}

int	insert_pos(t_exec *data, char *param)
{
	size_t	iter;
	size_t	end_of_name;

	end_of_name = 0;
	while (param[end_of_name] != '=')
		end_of_name++;
	iter = 0;
	while (data->envp[iter])
	{
		if (!ft_strncmp(data->envp[iter], param, end_of_name + 1))
			return (iter);
		iter++;
	}
	iter = 0;
	while (data->envp[iter])
	{
		if (!ft_strcmp(data->envp[iter], ""))
			return (iter);
		iter++;
	}
	return (-1);
}

int	insert_env(t_exec *data, char *entry)
{
	char	*new;
	size_t	position;

	new = ft_strdup(entry);
	if (!new)
		return (perror("export"), -1);
	position = insert_pos(data, entry);
	if (position == -1)
	{
		if (data->envp_count >= data->envp_malloc)
			if (extend_envp(data) == -1)
				return (perror("export"), free(new), -1);
		data->envp[data->envp_count] = new;
		data->envp_count = data->envp_count + 1;
		data->envp[data->envp_count] = NULL;
	}
	else
	{
		free(data->envp[position]);
		data->envp[position] = new;
	}
	return (0);
}

int	export(char **cmd, t_exec *data)
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
		if (insert_env(data, cmd[iter]) == -1)
			return (-1);
		iter++;
	}
	return (return_value);
}
