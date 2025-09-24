/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   own_cmds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 20:15:44 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/24 11:33:06 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	return (0);
}
void	pwd(t_exec *data, t_cmd *cmd)
{
	char	*buf;

	buf = getcwd(NULL, 0);
	if (!buf)
	{
		perror("getcwd");
		child_exit_handle(data, cmd, 1);
	}
	if (write(1, buf, ft_strlen(buf)) == -1)
	{
		perror("write");
		child_exit_handle(data, cmd, 1);
	}
	if (write(1, "\n", 1) == -1)
	{
		perror("write");
		child_exit_handle(data, cmd, 1);
	}
	child_exit_handle(data, cmd, 0);
}

int	exit_cmd(t_exec *data, t_cmd *cmd)
{
	if (!data->pipe_count)
		data->exit = 1;
	return (0);
	// if (cmd->cmd[1] == NULL && data->pipe_count == 0)
	// 	return (12);
	// else if (cmd->cmd[1] == NULL)
	// 	return (0);
	// else
	// 	return (2); // exit failed in pipe error code 2
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
		if (write(1, cmd->cmd[iter], ft_strlen(cmd->cmd[iter])) == -1)
		{
			perror("write");
			child_exit_handle(data, cmd, 1);
		}
		if (cmd->cmd[iter + 1])
		{
			if (write(1, " ", 1) == -1)
			{
				perror("write");
				child_exit_handle(data, cmd, 1);
			}
		}
		iter++;
	}
	if (nflag == 0)
	{
		if (write(1, "\n", 1) == -1)
		{
			perror("write");
			child_exit_handle(data, cmd, 1);
		}
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
		// if "" delete entry and dont copy
		{
			free(data->envp[iter]);
			data->envp_count--;
		}
		else
		{
			new_envp[new_iter] = data->envp[iter];
			new_iter++;
		}
		iter++;
	}
	new_envp[new_iter] = NULL;
	free(data->envp);
	data->envp = new_envp;
	return (0);
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
	inner = 1;
	while (cmd[iter])
	{
		if (!cmd[iter][0] || (!ft_isalpha(cmd[iter][0]) && cmd[iter][0] != '_'))
			return (0);
		while (cmd[iter][inner])
		{
			if (!cmd[iter][inner] || (!ft_isalpha(cmd[iter][inner]) && cmd[iter][inner] != '_'))
				return (0);
			inner++;
		}
		envp_pos = ft_find_paths(data->envp, cmd[iter]);
		if (envp_pos == -1)
			return (0);
		entry = ft_calloc(1, 1);
		if (!entry)
			return (perror("unset"), -1);
		free(data->envp[envp_pos]);
		data->envp[envp_pos] = entry;
		iter++;
	}
	return (0);
}

int	check_position(t_exec *data)
{
	size_t	iter;

	iter = 0;
	while (data->envp[iter])
	{
		if (!ft_strcmp(data->envp[iter], ""))
			return (iter);
		iter++;
	}
	return (-1);
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
			return (/* own error */ -1);
		inner = 1;
		while (cmd[iter][inner] != '=')
		{
			if (!ft_isalnum(cmd[1][inner]) && cmd[1][inner] != '_')
				return (/* own error */ -1);
			inner++;
		}
		if (cmd[iter][inner] == '=')
			inner++;
		value = ft_strdup(&cmd[iter][inner]);
		if (!value)
			return (-1);
		if (data->envp_count >= data->envp_malloc)
			if (extend_envp(data) == -1)
				return (free(value), -1);
		position = check_position(data);
		if (position == -1)
		{
			data->envp[data->envp_count] = value;
			data->envp_count = data->envp_count + 1;
			data->envp[data->envp_count] = NULL;
		}
		else
			data->envp[position] = value;
		iter++;
	}
	return (0);
}

void	env(char **envp, t_exec *data, t_cmd *cmd)
{
	size_t	iter;

	iter = 0;
	if (!envp)
		exit(1);
	while (envp[iter])
	{
		if (write(1, envp[iter], ft_strlen(envp[iter])) == -1)
		{
			perror("write");
			exit(1);
		}
		if (envp[iter][0] != '\0' && write(1, "\n", 1) == -1)
		{
			perror("write");
			exit(1);
		}
		iter++;
	}
	exit(0);
}
