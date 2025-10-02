/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_interactive.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:46:45 by jromann           #+#    #+#             */
/*   Updated: 2025/10/02 10:51:40 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	non_interactive(t_input *data)
{
	char	*buf;
	int		exit_code;
	size_t	len;

	setup_noninteractive_signals();
	while (1)
	{
		buf = get_next_line(STDIN_FILENO);
		if(!buf)
			break ;
		cut_nl(buf);
		if (empty_prompt(buf))
		{
			free(buf);
			continue ;
		}
		exit_code = parse_and_execute(buf, data, NONINTERACTIVE);
		if (exit_code || data->exit)
			return (free(buf), get_next_line(-1), exit(exit_code));
		free(buf);
	}
	free2d(&data->envp.vars);
	exit(data->exit_code);
}
