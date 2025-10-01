/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 19:10:38 by jromann           #+#    #+#             */
/*   Updated: 2025/10/01 09:39:12 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_pipe_pos(t_exec *data)
{
	t_entry	*iter;
	size_t	counter;
	size_t	p_iter;

	counter = 0;
	iter = data->list;
	data->pipe_position = malloc(sizeof(int) * (data->pipe_count + 1));
	if (!data->pipe_position)
		return (-1);
	data->pipe_position[0] = 0;
	p_iter = 1;
	while (iter != NULL)
	{
		if (iter->spec == PIPE)
		{
			data->pipe_position[p_iter] = counter + 1;
			p_iter++;
		}
		counter++;
		iter = iter->next;
	}
	return (0);
}

static int	init_data(t_exec *data, t_input *input, char **envp)
{
	data->envp = input->envp;
	data->envp_count = input->envp_count;
	data->envp_malloc = input->envp_malloc;
	data->list = input->entries;
	// data->input_spec = input->input_spec;
	// data->entries = input->entries;
	data->pipe_count = 0;
	data->exit = 0;
	data->exit_code = input->exit_code;
	// if (input->exp_str_malloc)
	// 	free(input->exp_str);
	if (here_doc(data) == -1)
    {
		if (g_current_signal != SIGINT)
            free2d(&data->envp);
	    return (-1);
    }
	if (init_pipe_pos(data) == -1)
		return (free2d(&data->heredoc), free2d(&data->envp), -1);
	return (0);
}

int	exec_central(t_input *input)
{
	t_exec	data;
	int		exit_code;

	exit_code = 0;
	if (init_data(&data, input, input->envp) == -1)
		return (-1);
	exit_code = execute_cmds(&data);
	// free(data.input_spec);
	// free2d(&data.entries);
	free(data.pipe_position);
	free2d(&data.heredoc);
	input->envp_count = data.envp_count;
	input->envp_malloc = data.envp_malloc;
	input->exit = data.exit;
	if (exit_code == -1)
		free2d(&data.envp);
	return (exit_code);
}
