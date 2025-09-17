/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 09:48:56 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/17 15:21:06 by eprottun         ###   ########.fr       */
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
			if (data->entries[iter + 1] && data->input_spec[iter + 1] == OPERATOR)
			{
				if (!ft_strcmp(data->entries[iter + 1], "<"))
					data->input_spec[iter] = DEFAULT_REDIRECT;
			}
		}
		iter++;
	}
}

int main(int argc, char *argv[], char *envp[])
{
	t_input data;

	char *buf;

	while ((buf = readline("minishell>> ")) != NULL)
	{
		    if (ft_strlen(buf) > 0) {
			{
				expand_input(buf, envp, &data);
				parse_string(data.expanded_str, &data);
				entry_spec(&data);
				size_t	iter = 0;
				while (data.entries[iter])
				{
					printf("%zu = [%s] (%i)\n", iter, data.entries[iter], data.input_spec[iter]);
					iter++;
				}
			}
		    //   add_history(buf);
		    }
		free(buf);
	}
	return (0);
}