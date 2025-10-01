/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_central.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 19:10:38 by jromann           #+#    #+#             */
/*   Updated: 2025/10/01 19:07:55 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	count_pipes(t_exec *data)
{
	t_entry	*node;

	node = data->list;
	while (node != NULL)
	{
		if (node->spec == PIPE)
			data->pipe_count++;
		node = node->next;
	}
}
static int	init_pipe_pos(t_exec *data)
{
	t_entry	*node;
	size_t	counter;
	size_t	iter;

	iter = 0;
	count_pipes(data);
	data->pipe_position = malloc(sizeof(int) * (data->pipe_count + 1));
	if (!data->pipe_position)
		return (-1);
	data->pipe_position[iter++] = 0;
	counter = 0;
	node = data->list;
	while (node != NULL)
	{
		if (node->spec == PIPE)
			data->pipe_position[iter++] = counter + 1;
		counter++;
		node = node->next;
	}
	return (0);
}

static int	init_data(t_exec *data, t_input *input, char **envp)
{
	data->envp = input->envp;
	data->envp->count = input->envp->count;
	data->envp->malloc = input->envp->malloc;
	data->list = input->entries;
	data->pipe_count = 0;
	data->exit = 0;
	data->exit_code = input->exit_code;
	data->heredoc = input->heredoc;
	if (init_pipe_pos(data) == -1)
		return (free2d(&data->heredoc), free2d(&data->envp->vars), -1);
	return (0);
}

int	exec_central(t_input *input)
{
	t_exec	data;
	int		exit_code;

	exit_code = 0;
	if (init_data(&data, input, input->envp->vars) == -1)
		return (-1);
	exit_code = execute_cmds(&data);
	free(data.pipe_position);
	// free2d(&data.heredoc);
	input->envp->count = data.envp->count;
	input->envp->malloc = data.envp->malloc;
	input->exit = data.exit;
	if (exit_code == -1)
		free2d(&data.envp->vars);
	return (exit_code);
}
