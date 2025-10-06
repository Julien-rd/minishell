/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_central.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jromann <jromann@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 19:10:38 by jromann           #+#    #+#             */
/*   Updated: 2025/10/06 10:04:32 by jromann          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_central(t_sh *sh)
{
	int	exit_code;

	sh->exit = 0;
	exit_code = 0;
	exit_code = pipeline(sh);
	free_list(sh->entries);
	if (sh->heredoc)
		free2d(&sh->heredoc);
	return (exit_code);
}
