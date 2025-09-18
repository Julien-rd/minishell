/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 19:10:38 by jromann           #+#    #+#             */
/*   Updated: 2025/09/18 16:17:06 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	intiliase_pipe_pos(t_exec *data)
{
	size_t	iter;
	size_t	p_iter;
	int		pipe_count;

	iter = 0;
	p_iter = 0;
	if (data->pipe_count == 0)
		data->pipe_position = NULL;
	else
	{
		data->pipe_position = malloc(sizeof(char *) * data->pipe_count);
		if (!data->pipe_position)
			return (-1);
		while (data->input_spec[iter])
		{
			if (data->input_spec[iter] == PIPE)
			{
				data->pipe_position[p_iter] = iter;
				p_iter++;
			}
			iter++;
		}
	}
	return (0);
}
void	initiliase_data(t_exec *data, t_input *input, char **envp)
{
	data->envp = envp;
	data->input_spec = input->input_spec;
	data->entries = input->entries;
	data->fd[0] = 0;
	data->fd[1] = 0;
	data->pipe_iter = 0;
	data->pipe_count = 0;
	free(input->expanded_str);
	if (here_doc(data) == 1)
		return (1);
	initialise_pipe_pos(data);
	return (0);
}

int	exec_central(t_input *input, char **envp)
{
	t_exec data;

	initialise_data(&data, input, envp);
	execute_cmds(&data);
	return (0);
}