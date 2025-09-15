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

// int	double_quotes(char *buf, t_input *data, size_t *iter)
// {
// 	while (buf[*iter] != '"')
// 	{
// 		if (!buf[*iter])
// 			return (1); //bracket not closed
// 		if (buf[*iter] == '$')
// 			env_variable();
// 		(*iter)++;
// 	}
// }

// int	single_quotes(char *buf, t_input *data, size_t *iter)
// {
// 	int		end_quote;
// 	size_t	tmp_iter;
// 	char	*return_str;

// 	tmp_iter = *iter + 1;
// 	end_quote = 1;
// 	while (buf[tmp_iter] != '\'')
// 	{
// 		if (!buf[tmp_iter])
// 			end_quote = 0;
// 		(tmp_iter)++;
// 	}
// 	if (end_quote == 1)
// 	{
// 		return_str = ft_substr(buf, *iter + 1, tmp_iter - 1 - *iter);
// 		if (!return_str)
// 			return (-1);
// 	}
// 	else
// 	{
// 		tmp_iter = *iter;
// 		while (buf[tmp_iter] && buf[tmp_iter] != ' ')
// 			tmp_iter++;
// 		return_str = ft_substr(buf, *iter, tmp_iter - *iter);
// 		if (!return_str)
// 			return (-1);
// 	}
// }

int	is_token(char c)
{
	if (c == 32 || (c >= 9 && c <=13) || c == '>' || c == '<' || c == '|')
		return (0);
	return (1);
}

int	is_closed(char *str)
{
	size_t	iter;
	int		tmp_sgl_quote;

	iter = 0;
	tmp_sgl_quote = 0;
	while (str[iter])
	{
		if ()
		{
			/* code */
		}
		
	}
	
}

size_t	count_entries(char *buf, t_input *data)
{
	size_t	iter;
	size_t	count;

	iter = 0;
	count = 0;
	data->dbl_quote = 0;
	data->sgl_quote = 0;
	while (buf[iter])
	{
		while (buf[iter] && !is_token(buf[iter]))
			iter++;
		count++;
		while (data->dbl_quote || data->sgl_quote || (buf[iter] && is_token(buf[iter])))
		{
			if (data->sgl_quote == 0 && buf[iter] == '\'' && ft_strchr(&buf[iter + 1], '\''))
				data->sgl_quote = 1;
			else if (data->sgl_quote == 1 && buf[iter] == '\'')
				data->sgl_quote = 0;
			if (data->sgl_quote == 0 && data->dbl_quote == 0 && buf[iter] == '\"' && ft_strchr(&buf[iter + 1], '\"'))
				data->dbl_quote = 1;
			else if (data->sgl_quote == 0 && data->dbl_quote == 1 && buf[iter] == '\"')
				data->dbl_quote = 0;
			iter++;
		}
	}
	return (count);
}

char	**parse_string(char *buf, t_input *data)
{
	char	**entries;

	data->total_entries = count_entries(buf, data);
	printf("%zu", data->total_entries);
	return (NULL);
}

int main(int argc, char *argv[], char *envp[])
{
	t_input data;

	char *buf;
	data.envp = envp;

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