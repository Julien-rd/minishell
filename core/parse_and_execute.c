/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_execute.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 10:51:19 by jromann           #+#    #+#             */
/*   Updated: 2025/10/01 12:26:26 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	parse_and_execute(char *buf, t_input *data, int flag)
// {
// 	(void)flag;

// 	// while (data->entries)
// 	// {
// 	// 	printf("[%s][%d]\n", data->entries->raw_entry, data->entries->spec);
// 	// 	data->entries = data->entries->next;
// 	// }
// 	return (0);
// }

int	parse_and_execute(char *buf, t_input *data, int flag)
{
	size_t	len;
	size_t	iter;

	len = ft_strlen(buf);
	if (len == 0)
		return (0);
	if (flag == NONINTERACTIVE)
		buf[len - 1] = '\0';
	if (ultimate(buf, data) == -1)
		return (-1);
	// t_entry	*tmp = data->entries;
    // while (tmp != NULL) 
	// {
	// 	int i = -1;
	// 	if (tmp->expanded)
	// 		while(tmp->expanded[++i])
	// 			printf("%s\n", tmp->expanded[i]);
    //     tmp = tmp->next;
    // }
	if (syntax_check(data) == -1)
		return (data->exit_code = 2, 0);
	data->exit_code = exec_central(data);
	if (data->exit_code == -1 && g_current_signal == 0)
		return (perror("execution error"), -1);
	if (data->exit)
		return (data->exit_code);
	if (flag == INTERACTIVE)
		add_history(buf);
	return (0);
}
