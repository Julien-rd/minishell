/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_messages.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:58:02 by jromann           #+#    #+#             */
/*   Updated: 2025/09/25 17:00:03 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	invalid_option(t_exec *data, t_cmd *cmd)
{
	write(2, cmd->cmd[0], ft_strlen(cmd->cmd[0]));
    write(2, " ", 1);
	write(2, &cmd->cmd[1][0], 1);
	if (cmd->cmd[1][1])
		write(2, &cmd->cmd[1][1], 1);
	write(2, ": invalid option\n", 17);
	child_exit_handle(data, cmd, 2);
}