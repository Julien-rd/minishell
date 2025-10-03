/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_execute.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 10:51:19 by jromann           #+#    #+#             */
/*   Updated: 2025/10/03 10:23:18 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_and_execute(char *buf, t_sh *sh, int flag)
{
	size_t	len;
	size_t	iter;

	if (parsing(buf, sh) == -1)
		return (-1);
	if (syntax_check(sh) == -1)
		return (free_list(sh->entries), sh->exit_code = 2, 0);
	sh->exit_code = exec_central(sh);
	if (sh->exit_code == -1 && g_current_signal == 0)
		return (perror("execution error"), -1);
	if (sh->exit)
		return (sh->exit_code);
	if (flag == INTERACTIVE)
		add_history(buf);
	return (0);
}
