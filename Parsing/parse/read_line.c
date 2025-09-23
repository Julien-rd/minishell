/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 09:48:56 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/23 11:03:53 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	sigint_handler(int num)
{
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
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

/* parse_string - three malloc: entry**, input_spec*, entries */
/* entry_spec: no error */
int	main(int argc, char *argv[], char *envp[])
{
	t_input	data;
	char	*buf;

	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	if (create_envp(&data, envp) == -1)
		return (perror("envp"), free2d(data.envp), 1);
	data.exit_code = 0;
	while (1)
	{
		buf = readline("minishell>> ");
		if (buf == NULL)
			break ;
		if (ft_strlen(buf) > 0)
		{
			if (expand_input(buf, data.envp, &data) == -1)
				return (perror("expand_input"), free2d(data.envp), 1);
			if (parse_string(&data) == -1)
				return (1);
			data.exit_code = exec_central(&data, envp);
			if (data.exit_code == -1)
				return (perror("execution error"), 1);
			if (data.exit_code == 12)
				return (write(1, "exit\n", 5), 0);
			add_history(buf);
		}
		free(buf);
	}
	free2d(data.envp);
	return (write(1, "exit\n", 5), 0);
	return (0);
}
