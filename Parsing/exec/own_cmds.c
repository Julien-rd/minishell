/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   own_cmds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 20:15:44 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/22 15:36:05 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	cd(t_cmd *cmd)
{
	size_t	iter;

	while (cmd->cmd[iter])
		iter++;
	if (iter > 2)
	{
		write(2, "cd: too many arguments", 22);
		write(2, "\n", 1);
		return (-1);
	}
	else if (chdir(cmd->cmd[1]) == -1)
	{
		perror("cd");
		return(-1);
	}
	// Error handling chdir kackt ab// path not found
	return(0);
}
void	pwd(void)
{
	char	*buf;

	buf = getcwd(NULL, 0);
	if (!buf)
	{
		perror("getcwd");
		exit(1);
	}
	write(1, buf, ft_strlen(buf));
	exit(0);
}

void	exit_cmd(t_exec *data, t_cmd *cmd)
{
	size_t	iter;

	iter = 1;
	if (cmd->cmd[1] == NULL && data->pipe_count == 0)
	{
		write(1, "exit", 5);
		write(2, "\n", 1);
		exit(12);
	}
	else if (cmd->cmd[1] == NULL)
		exit(0);
	else
	{
		write(2, "exit : invalid option --", 24);
		while (cmd->cmd[iter])
		{
			write(2, " ", 1);
			write(2, cmd->cmd[iter], ft_strlen(cmd->cmd[iter]));
			iter++;
		}
		write(2, "\n", 1);
		exit(2); // exit failed in pipe error code 2
	}
}

void	echo(char **cmd, int nflag)
{
	size_t	iter;

	iter = 1;
	if (nflag == 0)
		return ;
	if (nflag == 1)
		iter = 2;
	write(1, "HALLO\n", 6);
	while (cmd[iter])
	{
		if (write(1, cmd[iter], ft_strlen(cmd[iter])) == -1)
		{
			perror("write");
			exit(1);
		}
		if (cmd[iter + 1])
			if (write(1, " ", 1) == -1)
			{
				perror("write");
				exit(1);
			}
		iter++;
	}
	if (nflag == 2)
		if (write(1, "\n", 1) == -1)
		{
			perror("write");
			exit(1);
		}
	exit(0);
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
	char	*value;
	int		envp_pos;

	if (!cmd[1] || cmd[2] != NULL)
		return (/* own error */ -1);
	envp_pos = ft_find_paths(data->envp, cmd[1]);
	if (envp_pos == -1)
		return (/* own error */ -1);
	value = ft_strdup("");
	if (!value)
		return (perror("unset"), -1);
	if (data->envp_count >= data->envp_malloc)
		if (extend_envp(data) == -1)
			return (free(value), perror("unset"), -1);
	data->envp[envp_pos] = value;
	exit(0);
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
	size_t	position;

	if (!cmd[1] || cmd[2] != NULL || !ft_strchr(cmd[1], '=') || cmd[1][0] == '='
		|| (!ft_isalpha(cmd[1][0]) && cmd[1][0] != '_'))
		return (/* own error */ -1);
	iter = 0;
	while (cmd[1][iter] != '=')
	{
		if (!ft_isalnum(cmd[1][iter]) && cmd[1][iter] != '_')
			return (/* own error */ -1);
		iter++;
	}
	while (cmd[1][iter])
		iter++;
	value = ft_strdup(cmd[1]);
	if (!value)
		return (perror("export"), -1);
	if (data->envp_count >= data->envp_malloc)
		if (extend_envp(data) == -1)
			return (free(value), perror("export"), -1);
	position = check_position(data);
	if (position == -1)
	{
		data->envp[data->envp_count] = value;
		data->envp[++data->envp_count] = NULL;
	}
	else
		data->envp[position] = value;
	return (0);
}

void	env(char **envp)
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
		if (write(1, "\n", 1) == -1)
		{
			perror("write");
			exit(1);
		}
		iter++;
	}
	write(1, "jo", 2);
	exit(0);
}
