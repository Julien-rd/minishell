/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   own_cmds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 20:15:44 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/27 15:38:21 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd(t_exec *data, t_cmd *cmd, size_t pipe_count)
{
	size_t	iter;

	iter = 0;
	while (cmd->cmd[iter])
		iter++;
	if (iter > 2)
		return (-2);
	else if (!pipe_count && chdir(cmd->cmd[1]) == -1)
	{
		data->exit_code = errno;
		return (-1);
	}
	if(!pipe_count)
		pwd(data, cmd, 1);
	return (0);
}
void	pwd(t_exec *data, t_cmd *cmd, int flag)
{
	char	*buf;

	if (flag == -1)
		return ;
	buf = getcwd(NULL, 0);
	if (!buf)
	{
		perror("getcwd");
		child_exit_handle(data, cmd, 1);
	}
	if (flag == 1)
	{
		export(cmd->cmd, data); //OLDPWD
		export(cmd->cmd, data); //PWD
	}
	if (safe_write(1, buf, ft_strlen(buf)) == -1)
		return (free(buf), child_exit_handle(data, cmd, 1));
	if (safe_write(1, "\n", 1) == -1)
		return (free(buf), child_exit_handle(data, cmd, 1));
	return (free(buf), child_exit_handle(data, cmd, 0));
}

int	exit_cmd(t_exec *data, t_cmd *cmd)
{
	if (!data->pipe_count)
		data->exit = 1;
	return (0);
}

void	echo(t_exec *data, t_cmd *cmd, int nflag)
{
	size_t	iter;

	iter = 1;
	if (nflag == -1)
		return ;
	iter += nflag;
	while (cmd->cmd[iter])
	{
		if (safe_write(1, cmd->cmd[iter], ft_strlen(cmd->cmd[iter])) == -1)
			child_exit_handle(data, cmd, 1);
		if (cmd->cmd[iter + 1])
		{
			if (safe_write(1, " ", 1) == -1)
				child_exit_handle(data, cmd, 1);
		}
		iter++;
	}
	if (nflag == 0)
	{
		if (safe_write(1, "\n", 1) == -1)
			child_exit_handle(data, cmd, 1);
	}
	child_exit_handle(data, cmd, 0);
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

int	param_check(char *param)
{
	size_t	iter;

	iter = 0;
	while (param[iter])
	{
		if (!ft_isalnum(param[iter]) && param[iter] != '_')
			return (0);
		iter++;
	}
	return (1);
}

int	unset(char **cmd, t_exec *data)
{
	char	*entry;
	int		envp_pos;
	size_t	iter;
	size_t	inner;

	if (!cmd[1])
		return (0);
	iter = 1;
	while (cmd[iter])
	{
		if (!cmd[iter][0] || (!ft_isalpha(cmd[iter][0]) && cmd[iter][0] != '_'))
		{
			iter++;
			continue ;
		}
		inner = 1;
		if (!param_check(cmd[iter]))
		{
			iter++;
			continue ;
		}
		envp_pos = ft_find_paths(data->envp, cmd[iter]);
		if (envp_pos == -1)
		{
			iter++;
			continue ;
		}
		entry = ft_calloc(1, 1);
		if (!entry)
			return (perror("unset"), -1);
		free(data->envp[envp_pos]);
		data->envp[envp_pos] = entry;
		iter++;
	}
	return (0);
}

int	check_position(t_exec *data, char *value, size_t inner)
{
	size_t	iter;

	iter = 0;
	while (data->envp[iter])
	{
		if (!ft_strcmp(data->envp[iter], ""))
			return (iter);
		if (!ft_strncmp(data->envp[iter], value, inner))
			return (iter);
		iter++;
	}
	return (-1);
}

int	env_check(char *param, size_t *inner)
{
	while (param[*inner] && param[*inner] != '=')
	{
		if (!ft_isalnum(param[*inner]) && param[*inner] != '_')
			return (0);
		(*inner)++;
	}
	return (1);
}

int	export(char **cmd, t_exec *data)
{
	char	*value;
	size_t	iter;
	size_t	inner;
	size_t	position;

	iter = 1;
	while (cmd[iter])
	{
		if (!cmd[iter] || !ft_strchr(cmd[iter], '=') || cmd[iter][0] == '='
			|| (!ft_isalpha(cmd[iter][0]) && cmd[iter][0] != '_'))
		{
			iter++;
			continue ;
		}
		inner = 1;
		if (!env_check(cmd[iter], &inner))
		{
			iter++;
			continue ;
		}
		if (cmd[iter][inner] != '=')
		{
			iter++;
			continue ;
		}
		value = ft_strdup(cmd[iter]);
		if (!value)
			return (-1);
		if (data->envp_count >= data->envp_malloc)
			if (extend_envp(data) == -1)
				return (free(value), -1);
		position = check_position(data, value, inner + 1);
		if (position == -1)
		{
			data->envp[data->envp_count] = value;
			data->envp_count = data->envp_count + 1;
			data->envp[data->envp_count] = NULL;
		}
		else
		{
			free(data->envp[position]);
			data->envp[position] = value;
		}
		iter++;
	}
	return (0);
}

void	env(char **envp, t_exec *data, t_cmd *cmd, int flag)
{
	size_t	iter;

	iter = 0;
	if (!envp)
		child_exit_handle(data, cmd, 1);
	if (flag == -1)
		invalid_option(data, cmd);
	while (envp[iter])
	{
		if (safe_write(1, envp[iter], ft_strlen(envp[iter])) == -1)
			child_exit_handle(data, cmd, 1);
		if (envp[iter][0] != '\0' && safe_write(1, "\n", 1) == -1)
			child_exit_handle(data, cmd, 1);
		iter++;
	}
	child_exit_handle(data, cmd, 0);
}
