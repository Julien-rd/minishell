/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 09:48:56 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/29 12:33:16 by jromann          ###   ########.fr       */
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
		if (data->input_spec[iter] == OPERATOR)
		{
			if (!ft_strcmp(data->entries[iter], "<<"))
			{
				data->input_spec[iter] = HERE_DOC_OP;
				if (data->input_spec[iter + 1] == DEFAULT)
					data->input_spec[iter + 1] = HERE_DOC;
			}
			else if (!ft_strcmp(data->entries[iter], ">>"))
			{
				data->input_spec[iter] = APPEND_OP;
				if (data->input_spec[iter + 1] == DEFAULT)
					data->input_spec[iter + 1] = APPEND_FILE;
			}
			else if (!ft_strcmp(data->entries[iter], "<"))
			{
				data->input_spec[iter] = INFILE_OP;
				if (data->input_spec[iter + 1] == DEFAULT)
					data->input_spec[iter + 1] = INFILE;
			}
			else if (!ft_strcmp(data->entries[iter], ">"))
			{
				data->input_spec[iter] = OUTFILE_OP;
				if (data->input_spec[iter + 1] == DEFAULT)
					data->input_spec[iter + 1] = OUTFILE;
			}
			else if (!ft_strcmp(data->entries[iter], "|"))
				data->input_spec[iter] = PIPE;
		}
		iter++;
	}
}

int	create_envp(t_input *data, char *envp[])
{
	size_t	iter;
	size_t	create_iter;

	if (!envp)
		return (write(1, "create_envp: envp not found\n", 29), -1);
	iter = 0;
	while (envp[iter])
		iter++;
	data->envp_count = iter;
	data->envp_malloc = iter * 2;
	data->envp = malloc((data->envp_malloc + 1) * sizeof(char *));
	if (!data->envp)
		return (perror("create_envp"), -1);
	create_iter = 0;
	while (create_iter < iter)
	{
		data->envp[create_iter] = ft_strdup(envp[create_iter]);
		if (!data->envp[create_iter])
			return (perror("create_envp"), free2d(&data->envp), -1);
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
		return (1);
	data.exit_code = 0;
	data.exit = 0;
	if (!isatty(STDIN_FILENO))
		non_interactive(&data);
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
			return (free(buf), free2d(&data.envp), 1);
		if (data.exit)
			return (free(buf), safe_write(1, "exit\n", 5), free2d(&data.envp),
				exit_code);
		free(buf);
	}
	rl_clear_history();
	return (free2d(&data.envp), safe_write(1, "exit\n", 5), 0);
}
