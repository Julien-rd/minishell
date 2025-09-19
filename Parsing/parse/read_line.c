/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 09:48:56 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/19 15:00:30 by eprottun         ###   ########.fr       */
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
		if (data->input_spec[iter] == OPERATOR && !ft_strcmp(data->entries[iter], "|"))
			data->input_spec[iter] = PIPE;
		iter++;
	}
}

int main(int argc, char *argv[], char *envp[])
{
	t_input data;

	char *buf;

	while ((buf = readline("minishell>> ")) != NULL)
	{
		if (ft_strlen(buf) > 0)
		{
			expand_input(buf, envp, &data);
			if (parse_string(&data) == -1) //three malloc: entry**, input_spec*, entries
				return (perror("parsing"), 1);
			entry_spec(&data); //no error
			exec_central(&data, envp);
			add_history(buf);
		}
		free(buf);
	}
	return (0);
}
