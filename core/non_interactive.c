/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   non_interactive.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 16:46:45 by jromann           #+#    #+#             */
/*   Updated: 2025/09/29 09:51:00 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	non_interactive(t_input *data)
{
	char	*buf;
	int		exit_code;
	size_t	len;

	setup_noninteractive_signals();
	buf = get_next_line(STDIN_FILENO);
	while (buf)
	{
		exit_code = parse_and_execute(buf, data, NONINTERACTIVE);
		if (exit_code || data->exit)
			return (get_next_line(-1), exit(exit_code));
		buf = get_next_line(STDIN_FILENO);
	}
	free2d(&data->envp);
	exit(data->exit_code);
}
