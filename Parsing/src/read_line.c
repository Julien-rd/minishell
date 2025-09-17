/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 09:48:56 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/17 11:43:11 by jromann          ###   ########.fr       */
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

int	main(int argc, char *argv[], char *envp[])
{
	t_input data;

	char *buf;
	data.envp = envp;

	while ((buf = readline("minishell>> ")) != NULL)
	{
		heredoc()
		// token(buf, envp, &data);
		// if (ft_strlen(buf) > 0) {
		// 	parse_string(buf, &data);
		// //   add_history(buf);
		// }
		free(buf);
	}
	return (0);
}