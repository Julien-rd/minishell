/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 09:48:56 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/28 14:49:11 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile int	g_current_signal = 0;

void	entry_spec(t_input *data)
{
	size_t	iter;

	iter = 0;
	while (data->entries[iter])
	{
		if (data->input_spec[iter] == DEFAULT)
		{
			if (iter != 0 && data->input_spec[iter - 1] == OPERATOR)
			{
				if (!ft_strcmp(data->entries[iter - 1], "<<"))
					data->input_spec[iter] = HERE_DOC;
				else if (!ft_strcmp(data->entries[iter - 1], ">>"))
					data->input_spec[iter] = APPEND_FILE;
				else if (!ft_strcmp(data->entries[iter - 1], "<"))
					data->input_spec[iter] = INFILE;
				else if (!ft_strcmp(data->entries[iter - 1], ">"))
					data->input_spec[iter] = OUTFILE;
			}
		}
		if (data->input_spec[iter] == OPERATOR
			&& !ft_strcmp(data->entries[iter], "|"))
			data->input_spec[iter] = PIPE;
		iter++;
	}
}

int	create_envp(t_input *data, char *envp[])
{
	size_t	iter;
	size_t	create_iter;

	if (!envp)
		return (-1);
	iter = 0;
	while (envp[iter])
		iter++;
	data->envp_count = iter;
	data->envp_malloc = iter * 2;
	data->envp = malloc((data->envp_malloc + 1) * sizeof(char *));
	if (!data->envp)
		return (-1);
	create_iter = 0;
	while (create_iter < iter)
	{
		data->envp[create_iter] = ft_strdup(envp[create_iter]);
		if (!data->envp[create_iter])
			return (-1);
		create_iter++;
	}
	data->envp[create_iter] = NULL;
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_input	data;
	char	*buf;
	int		exit_code;

	if (create_envp(&data, envp) == -1)
		return (perror("envp"), free2d(&data.envp), 1);
	data.exit_code = 0;
	data.exit = 0;
	if (!isatty(STDIN_FILENO))
		non_interactive(&data);
	setup_main_signals();
	while (1)
	{
		g_current_signal = 0;
		setup_interactive_signals();
		buf = readline("minishell>> ");
		if (buf == NULL)
			break ;
		setup_main_signals();
		exit_code = parse_and_execute(buf, &data, INTERACTIVE);
		if (exit_code == -1)
			return (1);
		if (data.exit)
			return (write(1, "exit\n", 5), exit_code);
	}
	rl_clear_history();
	return (free2d(&data.envp), write(1, "exit\n", 5), 0);
}
