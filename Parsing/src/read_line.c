/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 09:48:56 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/15 09:49:16 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	tmp_spec_def(char *buf, t_input *data, size_t *iter)
{
	size_t	tmp_iter;

	tmp_iter = iter;
	while (buf[tmp_iter] && !ft_isalpha(buf[tmp_iter]))
	{
		if (buf[tmp_iter] == '<')
			return (data->tmp_spec = INFILE,  0);
		if (buf[tmp_iter] == '>')
			return (data->tmp_spec = OUTFILE,  0);
		if (buf[tmp_iter] == '|')
			return (data->tmp_spec = CMD,  0);
		tmp_iter++;
	}
}

int	double_quotes(char *buf, t_input *data, size_t *iter)
{
	while (buf[*iter] != '"')
	{
		if (!buf[*iter])
			return (1); //bracket not closed
		if (buf[*iter] == '$')
			env_variable();
		(*iter)++;
	}
}

int	single_quotes(char *buf, t_input *data, size_t *iter)
{
	while (buf[*iter] != '\'')
	{
		if (!buf[*iter])
			return (1); //bracket not closed
		if (buf[*iter] == '$')
			env_variable();
		(*iter)++;
	}
}

int	parse_string(char *buf, t_input *data)
{
	size_t	iter;

	iter = 0;
	data->tmp_spec = CMD;
	data->cmd_count = 0;
	data->env_count = 0;
	data->file_count = 0;
	
	while (buf[iter])
	{
		tmp_spec_def(buf, data, &iter);
		if (buf[iter] == '\"')
			double_quotes();
		else if (buf[iter] == '\'')
			single_quotes();
		else
			normal_case();
	}
	return (0);
}

int main()
{
	t_input data;

	char *buf;
	while ((buf = readline("minishell>> ")) != NULL)
	{
		    if (ft_strlen(buf) > 0) {
				parse_string(buf, &data);
		    //   add_history(buf);
		    }
		free(buf);
	}
	return (0);
}