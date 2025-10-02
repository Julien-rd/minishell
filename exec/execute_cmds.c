/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:40:30 by eprottun          #+#    #+#             */
/*   Updated: 2025/10/02 12:00:49 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cmd_init(t_cmd *cmd)
{
	t_entry	*l_iter;
	size_t	cmd_tokens;
	size_t	cmd_iter;
	size_t	iter;

	l_iter = cmd->line;
	cmd_tokens = 0;
	while (l_iter && l_iter->spec != PIPE)
	{
		if (l_iter->spec == DEFAULT)
		{
			iter = -1;
			while(l_iter->expanded && l_iter->expanded[++iter])
				cmd_tokens++;
		}
		l_iter = l_iter->next;
	}
	cmd->cmd = malloc((cmd_tokens + 1) * sizeof(char *));
	if (!cmd->cmd)
		return (-1);
	l_iter = cmd->line;
	cmd_iter = 0;
	while (l_iter && l_iter->spec != PIPE)
	{
		if (l_iter->spec == DEFAULT)
		{
			iter = -1;
			while(l_iter->expanded && l_iter->expanded[++iter])
				cmd->cmd[cmd_iter++] = l_iter->expanded[iter];
		}
		l_iter = l_iter->next;
	}
	cmd->cmd[cmd_iter] = NULL;
	iter = -1;
	return (0);
}

void	child_exit_handle(t_sh *sh, t_cmd *cmd, int errcode)
{
	free2d(&sh->envp.vars);
	free_list(sh->entries);
	free(sh->pipe.position);
	free(cmd->cmd);
	if (sh->heredoc)
		free2d(&sh->heredoc);
	exit(errcode);
}

void	child_process(t_sh *sh, t_cmd *cmd)
{
	char	*path;
	int		flag;

	setup_child_signals();;
	if (setup_redirect(sh, cmd) == -1)
		child_exit_handle(sh, cmd, 1);
	if (cmd->cmd[0] == NULL)
		child_exit_handle(sh, cmd, 0);
	if (cmd->cmd_flag != EXTERNAL)
		builtin_handler(sh, cmd);
	path = ft_getpath(sh->envp.vars, cmd->cmd[0]); // splitfail malloc
	if (path == NULL || cmd->cmd[0][0] == 0)
		command_fail(path, sh, cmd);
	execve(path, cmd->cmd, sh->envp.vars);
	execve_fail(path, errno, sh, cmd);
}

void	parent_process(t_sh *sh)
{
	if (sh->pipe.iter != 0)
	{
		if (close(sh->pipe.prev_fd) == -1)
			exit(1);
	}
	if (sh->pipe.iter != sh->pipe.count)
	{
		sh->pipe.prev_fd = sh->pipe.fd[0];
		if (close(sh->pipe.fd[1]) == -1)
			exit(1);
	}
}

int	kill_children(t_sh *sh)
{
	int		status;
	int		return_value;
	pid_t	pid;

	return_value = -1;
	while (1)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
		{
			if (errno == ECHILD)
				break ;
			else if (errno == EINTR)
				continue ;
			else
				break ;
		}
		if (pid == sh->pipe.last_pid)
		{
			if (WIFEXITED(status))
				return_value = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				return_value = 128 + WTERMSIG(status);
			else if (WIFSTOPPED(status))
				return_value = 128 + WSTOPSIG(status);
		}
	}
	if (errno != ECHILD)
		return (perror("waitpid"), -1);
	return (return_value);
}

int	own_cmd_exec(t_sh *sh, t_cmd *cmd)
{
	int	flag;

	if (cmd->cmd[0] == NULL)
		return (0);
	flag = options_check(cmd);
	if (cmd->cmd_flag == CD && !flag)
		return (cd(sh, cmd, sh->pipe.count));
	if (cmd->cmd_flag == EXIT)
		return (exit_cmd(sh, cmd));
	if (cmd->cmd_flag == EXPORT && !flag)
		return (export(cmd->cmd, sh));
	if (cmd->cmd_flag == UNSET && !flag)
		return (unset(cmd->cmd, sh));
	return (0);
}
void find_start(t_cmd	*cmd, t_sh *sh)
{
	t_entry	*l_iter;
	size_t iter;
	
	iter = 0;
	l_iter = sh->entries;
	while(l_iter && iter < sh->pipe.position[sh->pipe.iter])
	{
		l_iter = l_iter->next;
		iter++;
	}
	cmd->line = l_iter;
}

int	execute_cmds(t_sh *sh)
{
	t_cmd	cmd;
	t_entry	*iter;

	while (sh->pipe.iter <= sh->pipe.count)
	{
		find_start(&cmd, sh);
		if (cmd_init(&cmd) == -1)
			return (perror("cmd_init"), -1);
		cmd_flag(sh, &cmd);
		if (sh->pipe.iter != sh->pipe.count)
			if (pipe(sh->pipe.fd) == -1)
				return (perror("pipe"), free(cmd.cmd), -1);
		sh->internal_errcode = own_cmd_exec(sh, &cmd);
		sh->pipe.pid = fork();
		if (sh->pipe.pid == -1)
			return (perror("fork"), free(cmd.cmd), -1);
		if (sh->pipe.iter == sh->pipe.count)
			sh->pipe.last_pid = sh->pipe.pid;
		if (sh->pipe.pid == 0)
			child_process(sh, &cmd);
		else
			parent_process(sh);
		sh->pipe.iter++;
		free(cmd.cmd);
	}
	return (kill_children(sh));
}
