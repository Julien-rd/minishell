/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 09:48:56 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/19 20:51:36 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	entry_spec(t_input	*data)
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
/* parse_string - three malloc: entry**, input_spec*, entries */
/* entry_spec: no error */
int	main(int argc, char *argv[], char *envp[])
{
	t_input	data;
	char	*buf;

	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		buf = readline("minishell>> ");
		if (buf == NULL)
			break ;
		if (ft_strlen(buf) > 0)
		{
			expand_input(buf, envp, &data);
			if (parse_string(&data) == -1)
				return (perror("parsing"), 1);
			entry_spec(&data);
			if (exec_central(&data, envp) == -1)
				return (perror("execution error"), 1);
			add_history(buf);
		}
		free(buf);
	}
	return (0);
}
