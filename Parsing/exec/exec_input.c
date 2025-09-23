/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 19:10:38 by jromann           #+#    #+#             */
/*   Updated: 2025/09/23 17:57:00 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	init_pipe_pos(t_exec *data)
{
	size_t	iter;
	size_t	p_iter;
	
	data->pipe_position = malloc(sizeof(int) * (data->pipe_count + 1));
	if (!data->pipe_position)
		return (-1);
	data->pipe_position[0] = 0;
	iter = 0;
	p_iter = 1;
	while (data->input_spec[iter] != END)
	{
		if (data->input_spec[iter] == PIPE)
		{
			data->pipe_position[p_iter] = iter + 1;
			p_iter++;
		}
		iter++;
	}
	return (0);
}

static int	init_data(t_exec *data, t_input *input, char **envp)
{
	data->envp = input->envp;
	data->envp_count = input->envp_count;
	data->envp_malloc = input->envp_malloc;
	data->input_spec = input->input_spec;
	data->entries = input->entries;
	data->pipe_count = 0;
	data->exit = 0;
	data->exit_code = input->exit_code;
	if (input->exp_str_malloc)
		free(input->exp_str);
	if (here_doc(data) == 1)
		return (free2d(data->envp), free(data->input_spec),
			free2d(data->entries), -1);
	if (init_pipe_pos(data) == -1)
		return (free2d(data->heredoc), free2d(data->envp),
			free(data->input_spec), free2d(data->entries), -1);
	return (0);
}

int	exec_central(t_input *input, char **envp)
{
	t_exec	data;
	int		exit_code;

	exit_code = 0;
	if (init_data(&data, input, envp) == -1)
		return (-1);
	exit_code = execute_cmds(&data);
	free(data.input_spec);
	free2d(data.entries);
	free(data.pipe_position);
	if (data.heredoc)
		free2d(data.heredoc);
	input->envp_count = data.envp_count;
	input->envp_malloc = data.envp_malloc;
	input->exit = data.exit;
	return (exit_code);
}
