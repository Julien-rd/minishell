/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_interactive.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:46:45 by jromann           #+#    #+#             */
/*   Updated: 2025/09/27 18:35:01 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	non_interactive(t_input *data)
{
	char	*buf;
	size_t	len;

	setup_noninteractive_signals();
	buf = get_next_line(STDIN_FILENO);
	while (buf)
	{
		len = ft_strlen(buf);
		if (len)
		{
			buf[len - 1] = '\0';
			if (expand(buf, data) == -1)
				return (free(buf), get_next_line(-1), perror("expand_input"),
					free2d(&data->envp), exit(1));
			if (parse_string(data) == -1)
				return (free(buf), get_next_line(-1), free2d(&data->envp),
					exit(1));
			data->exit_code = exec_central(data);
			if (data->exit_code == -1 && current_signal == 0)
				return (free(buf), get_next_line(-1), perror("execution error"),
					exit(1));
			if (data->exit)
				return (free(buf), get_next_line(-1), free2d(&data->envp),
					exit(data->exit_code));
		}
		free(buf);
		buf = get_next_line(STDIN_FILENO);
	}
	// get_next_line(-1);
	free2d(&data->envp);
	exit(0);
}
