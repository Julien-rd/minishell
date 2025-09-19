/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 19:10:38 by jromann           #+#    #+#             */
/*   Updated: 2025/09/19 15:42:28 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	init_pipe_pos(t_exec *data)
{
	size_t	iter;
	size_t	p_iter;
	int		pipe_count;

	iter = 0;
	
	data->pipe_position = malloc(sizeof(int) * (data->pipe_count + 1));
	if (!data->pipe_position)
		return (-1);
	data->pipe_position[0] = 0;
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
	data->pipe_count = p_iter - 1;
	return (0);
}
static int	init_data(t_exec *data, t_input *input, char **envp)
{
	data->envp = envp;
	data->input_spec = input->input_spec;
	data->entries = input->entries;
	data->pipe_iter = 0;
	data->pipe_count = 0;
	free(input->exp_str);
	if (here_doc(data) == 1)
		return (1);
	if (init_pipe_pos(data) == -1)
		return (-1);
	return (0);
}

int	exec_central(t_input *input, char **envp)
{
	t_exec data;
	
	if (init_data(&data, input, envp) == -1) //was macht return 1 oder 0 hier?
		return (-1);
	// if (data.heredoc)
		// printf("heredoc:%s\n", data.heredoc[0]);
	execute_cmds(&data);
	return (0);
}