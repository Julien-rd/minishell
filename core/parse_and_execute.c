/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_execute.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 10:51:19 by jromann           #+#    #+#             */
/*   Updated: 2025/09/28 14:52:02 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_and_execute(char *buf, t_input *data, int flag)
{
	size_t	len;

	len = ft_strlen(buf);
	if (len == 0)
		return (0);
	if (flag == NONINTERACTIVE)
		buf[len - 1] = '\0';
	if (expand(buf, data) == -1)
		return (free(buf), perror("expand_input"), free2d(&data->envp), -1);
	if (parse_string(data) == -1)
		return (free(buf), free2d(&data->envp), -1);
	data->exit_code = exec_central(data);
	if (data->exit_code == -1 && g_current_signal == 0)
		return (free(buf), perror("execution error"), -1);
	if (data->exit)
		return (free(buf), free2d(&data->envp), data->exit_code);
	if (flag == INTERACTIVE)
		add_history(buf);
	free(buf);
	return (0);
}
